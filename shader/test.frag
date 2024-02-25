#version 330 core
out vec4 color;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float greyIntensity;

void main()
{
    vec4 FragColor = texture(texture1, TexCoord);
    float greyScale = dot(FragColor.rgb,vec3(.5));
    color = vec4(FragColor.rgb*(1-greyIntensity) + (vec3(greyScale)*greyIntensity),FragColor.a);
}
