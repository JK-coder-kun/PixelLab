#version 330 core
out vec4 color;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float greyIntensity;

void main()
{
    //the lower the value of Pixels, the more pixelize
    float Pixels = greyIntensity *2048.0;
        float dx = 15.0 * (1.0 / Pixels);
        float dy = 10.0 * (1.0 / Pixels);
        vec2 Coord = vec2(dx * floor(TexCoord.x / dx),
                          dy * floor(TexCoord.y / dy));
    color = texture(texture1, Coord);

}
