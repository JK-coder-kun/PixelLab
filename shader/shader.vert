#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float rotateRad;

//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    float w = rotateRad;
    mat3 A = mat3(cos(w), -sin(w), 0.0,
                  sin(w),  cos(w), 0.0,
                  0.0,     0.0,    1.0);
    gl_Position = vec4(A*aPos, 1.0);
    //ourColor = aColor;
    TexCoord = aTexCoord;
}
