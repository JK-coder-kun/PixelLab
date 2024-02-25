#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

float quadVertices[] = {
       // positions          // colors           // texture coords
         0.6f,  0.6f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.6f, -0.6f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.6f, -0.6f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.6f,  0.6f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };


int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pixel Lab", NULL, NULL);
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
    glewInit();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsClassic();

    bool show_demo_window = false;
    bool show_another_window = false;
    static float greyIntensity = 0.0f;
    static float pixelBlocks = 0.0f;
    ImVec4 clear_color = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);


    // build and compile our shader program
    Shader ourShader("./shader/shader.vert", "./shader/shader.frag");


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

    //ourShader.use(); // don't forget to activate/use the shader before setting uniforms!

    unsigned int fbo,fbo_texture;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glGenTextures(1, &fbo_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER,0);


     unsigned int quadVAO, quadVBO,quadEBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        //created framebuffer object
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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
        glBindVertexArray(0);

        // default framebuffer object
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,windowWidth,windowHeight);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //GUI window
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
                    bool success = Texture::SaveAsImage(path,0,0,texture.GetWidth(), texture.GetHeight());
                    if(!success) std::cout<<"Unable to save image"<<std::endl;
                    else std::cout<<"Image has been saved!"<<std::endl;
                    glBindFramebuffer(GL_FRAMEBUFFER,0);
                }else std::cout<<"No path was given to save!"<<std::endl;
            }

            if(ImGui::Button("Exit"))
                glfwSetWindowShouldClose(window, true);

            ImGui::End();
        }

        //load image and create a texture
        if(newTextureSet)
        {
            imgPath = openFileDialog();
            texture.LoadTexture(imgPath);

            setRenderCoordinate(texture.GetWidth(), texture.GetHeight());
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            newTextureSet = false;

            //regenerate fbo to renew width and height of the texture that is attached to fbo
            glDeleteFramebuffers(1,&fbo);
            glDeleteTextures(1,&fbo_texture);

            glGenFramebuffers(1,&fbo);
            glBindFramebuffer(GL_FRAMEBUFFER,fbo);

            glGenTextures(1, &fbo_texture); // Generate one texture
            glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window

            // Setup the basic texture parameters
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
            glBindFramebuffer(GL_FRAMEBUFFER,0);
        }

        if(textureActive && windowResize)
        {
            setRenderCoordinate(texture.GetWidth(), texture.GetHeight());
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            windowResize = false;
        }


        //container to render our created fbo
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo_texture);	// use the color attachment texture as the texture of the quad plane
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteFramebuffers(1,&fbo);
    glDeleteVertexArrays(1,&quadVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &quadEBO);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

//set quad coordinate to match image ratio
void setRenderCoordinate(int width, int height)
{
    float initialCoord[] = {
        // positions          // colors           // texture coords
         0.6f,  0.6f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.6f, -0.6f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.6f, -0.6f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.6f,  0.6f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    std::copy(initialCoord, initialCoord+sizeof(initialCoord)/sizeof(initialCoord[0]), quadVertices);

    //if image width is greater, set quad width default and adjust height
    if(width >= height)
    {
        float winHWRatio = (float)windowWidth/(float)windowHeight;
        float hwRatio = (float)1.2f * winHWRatio* ((float)height/(float)width);
        std::cout<<"WinHW: "<<winHWRatio<<std::endl;
        std::cout<<"width: "<<width<<", height: "<<height<<", hwRatio: "<<hwRatio/1.2f<<std::endl;
        quadVertices[1]=0 + hwRatio/2;
        quadVertices[25]=quadVertices[1];
        quadVertices[9]= 0 - hwRatio/2;
        quadVertices[17] = quadVertices[9];
        std::cout<<"width: "<<(quadVertices[0]-quadVertices[16])<<", height: "<<(quadVertices[1]-quadVertices[9])<<", hwRatio: "<<(quadVertices[1]-quadVertices[9])/(quadVertices[0]-quadVertices[16])<<std::endl;


    }else  //if image height is greater, set quad height default and adjust width
    {
        float winWHRatio = (float)windowHeight/(float)windowWidth;
        float whRatio = (float)1.2f * winWHRatio*((float)width/(float)height);
        std::cout<<"WinWH: "<<winWHRatio<<std::endl;
        std::cout<<"width: "<<width<<", height: "<<height<<", whRatio: "<<whRatio/1.2f<<std::endl;
        quadVertices[0] = 0 + whRatio/2;
        quadVertices[8] = quadVertices[0];
        quadVertices[16]= 0 - whRatio/2;
        quadVertices[24]= quadVertices[16];
        std::cout<<"width: "<<(quadVertices[0]-quadVertices[16])<<", height: "<<(quadVertices[1]-quadVertices[9])<<", whRatio: "<<(quadVertices[0]-quadVertices[16])/(quadVertices[1]-quadVertices[9])<<std::endl;

    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed, this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    windowResize = true;

}


