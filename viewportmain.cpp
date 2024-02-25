
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
//#include <stdio.h>
//#include "glad/glad.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader/shader_s.h"
#include "Texture.h"
#include <iostream>
#include <windows.h>
#include <commdlg.h>

#include "fileDialog.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setRenderCoordinate(int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int windowWidth = SCR_WIDTH;
unsigned int windowHeight = SCR_HEIGHT;

string imgPath;
bool textureActive, newTextureSet, windowResize = false;
Texture texture;

float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int VBO, VAO, EBO;

//for debug
string texWidth,texHeight;
string texdimension;


int main()
{
    //openFileDialog();
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PIXEL LAB", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    glewInit();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsClassic();

    bool show_demo_window = false;
    bool show_another_window = false;
    static float greyIntensity = 0.0f;
    static float pixelBlocks = 0.0f;
    ImVec4 clear_color = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);

    //string imgPath;
    //bool textureActive, newTextureSet;
    //Texture texture;


    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("./shader/test.vert", "./shader/shader.frag");


    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader.use();

    unsigned int fbo,fbo_texture;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glGenTextures(1, &fbo_texture); // Generate one texture
    glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window

    // Setup the basic texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glViewport(0,0,texture.GetWidth(),texture.GetHeight());
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if(textureActive)texture.Bind();

        // render container
        ourShader.use();
        ourShader.setFloat("greyIntensity", greyIntensity);
        ourShader.setFloat("pixelBlocks", pixelBlocks);

        glBindVertexArray(VAO);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,windowWidth,windowHeight);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfwGL3_NewFrame();

        {   ImGui::Begin("Menu");
            ImGui::SetWindowPos(ImVec2(10,10),ImGuiCond_FirstUseEver);
            ImGui::Text("Welcome to Pixel Lab!");                           // Display some text (you can use a format string too)
            if(ImGui::Button("Load Image"))
            {
                pixelBlocks = 0.0f;
                greyIntensity = 0.0f;
                newTextureSet = true;
                textureActive = true;
            }
            ImGui::SliderFloat("Pixel Blocks", &pixelBlocks, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("GreyScale", &greyIntensity, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


            if(ImGui::Button("Save"))
            {
                char szFile[MAX_PATH] = {0};
                if(saveFileDialog(szFile))
                {
                    string path = convertToString(szFile, MAX_PATH) + ".png";
                    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
                    bool success = Texture::SaveAsImage(path,vertices[24],vertices[25],texture.GetWidth(), texture.GetHeight());
                    if(!success) std::cout<<"Unable to save image"<<std::endl;
                    else std::cout<<"Image has been saved!"<<std::endl;
                    glBindFramebuffer(GL_FRAMEBUFFER,0);
                }else std::cout<<"No path was given to save!"<<std::endl;
            }

            if(ImGui::Button("Exit"))
                glfwSetWindowShouldClose(window, true);

            ImGui::End();
        }

        //load and create a texture
        if(newTextureSet)
        {
            imgPath = openFileDialog();
            texture.LoadTexture(imgPath);

            setRenderCoordinate(texture.GetWidth(), texture.GetHeight());
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            newTextureSet = false;
        }


        if(textureActive && windowResize)
        {
            setRenderCoordinate(texture.GetWidth(), texture.GetHeight());
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            windowResize = false;
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

//set coordinate to match image ratio
void setRenderCoordinate(int width, int height)
{
    float initialCoord[] = {
        // positions          // colors           // texture coords
         0.6f,  0.6f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.6f, -0.6f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.6f, -0.6f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.6f,  0.6f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    std::copy(initialCoord, initialCoord+sizeof(initialCoord)/sizeof(initialCoord[0]), vertices);
    if(width >= height)
    {
        float winHWRatio = (float)windowWidth/(float)windowHeight;
        float hwRatio = (float)1.2f * winHWRatio* ((float)height/(float)width);
        std::cout<<"WinHW: "<<winHWRatio<<std::endl;
        std::cout<<"width: "<<width<<", height: "<<height<<", hwRatio: "<<hwRatio/1.2f<<std::endl;
        vertices[1]=0 + hwRatio/2;
        vertices[25]=vertices[1];
        vertices[9]= 0 - hwRatio/2;
        vertices[17] = vertices[9];
        std::cout<<"width: "<<(vertices[0]-vertices[16])<<", height: "<<(vertices[1]-vertices[9])<<", hwRatio: "<<(vertices[1]-vertices[9])/(vertices[0]-vertices[16])<<std::endl;


    }else
    {
        float winWHRatio = (float)windowHeight/(float)windowWidth;
        float whRatio = (float)1.2f * winWHRatio*((float)width/(float)height);
        std::cout<<"WinWH: "<<winWHRatio<<std::endl;
        std::cout<<"width: "<<width<<", height: "<<height<<", whRatio: "<<whRatio/1.2f<<std::endl;
        vertices[0] = 0 + whRatio/2;
        vertices[8] = vertices[0];
        vertices[16]= 0 - whRatio/2;
        vertices[24]= vertices[16];
        std::cout<<"width: "<<(vertices[0]-vertices[16])<<", height: "<<(vertices[1]-vertices[9])<<", whRatio: "<<(vertices[0]-vertices[16])/(vertices[1]-vertices[9])<<std::endl;

    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        char szFile[MAX_PATH] = {0};
        if(saveFileDialog(szFile))
        {
            string path = convertToString(szFile, MAX_PATH) + ".png";
            bool success = true;//Texture::SaveAsImage(path,window);
            if(!success) std::cout<<"Unable to save image"<<std::endl;
            else std::cout<<"Image has been saved!"<<std::endl;
        }else std::cout<<"No path was given to save!"<<std::endl;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    windowResize = true;

}


