#version 330 core
out vec4 finalColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float greyIntensity;
uniform float pixelBlocks;

void main()
{
    float Pixels = (1-pixelBlocks) *4096;
    float dx = 15.0 * (1.0 / Pixels);
    float dy = 15.0 * (1.0 / Pixels);
    vec2 Coord = vec2(dx * floor(TexCoord.x / dx),
                      dy * floor(TexCoord.y / dy));
    if(Pixels == 4096)Coord = vec2(TexCoord.x,TexCoord.y);
    vec4 OriginColor = texture(texture1, Coord);
    float greyScale = dot(OriginColor.rgb,vec3(.299,.587,.114));
    vec4 color = vec4(OriginColor.rgb*(1-greyIntensity) + (vec3(greyScale)*greyIntensity),OriginColor.a);
    finalColor = color;
}
