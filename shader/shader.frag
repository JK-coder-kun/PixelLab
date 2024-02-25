#version 330 core
out vec4 color;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float greyIntensity;
uniform float pixelBlocks;

void main()
{
    float Pixels = (1-pixelBlocks) *2048.0;
    float dx = 15.0 * (1.0 / Pixels);
    float dy = 10.0 * (1.0 / Pixels);
    vec2 Coord = vec2(dx * floor(TexCoord.x / dx),
                      dy * floor(TexCoord.y / dy));
    if(Pixels == 2048.0)Coord = vec2(TexCoord.x,TexCoord.y);
    vec4 FragColor = texture(texture1, Coord);
    float greyScale = dot(FragColor.rgb,vec3(.299,.587,.114));
    color = vec4(FragColor.rgb*(1-greyIntensity) + (vec3(greyScale)*greyIntensity),FragColor.a);
}
