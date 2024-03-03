#version 330 core
out vec4 finalColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float intensity;
uniform float pixelBlocks;
uniform int theme;
vec4 convertToGreyScale(vec4 OriginColor)
{
    float greyScale = dot(OriginColor.rgb,vec3(.299,.587,.114));
    return vec4(OriginColor.rgb*(1-intensity) + (vec3(greyScale)*intensity),OriginColor.a);
}
vec4 convertToPalette(vec4 OriginColor, vec4 theme_color)
{
    vec3  saturateTheme = vec3(2*(intensity+0.3));
    return vec4(OriginColor.rgb*saturateTheme,OriginColor.a);
}

void main()
{
    float Pixels = (1-pixelBlocks) *2048.0;
    float dx = 15.0 * (1.0 / Pixels);
    float dy = 15.0 * (1.0 / Pixels);
    vec2 Coord = vec2(dx * floor(TexCoord.x / dx),
                      dy * floor(TexCoord.y / dy));
    if(Pixels == 2048.0)Coord = vec2(TexCoord.x,TexCoord.y);
    vec4 OriginColor = texture(texture1, Coord);
    switch(theme){
        case 1: finalColor = convertToGreyScale(OriginColor);
                break;
        case 2: finalColor = convertToPalette(OriginColor, vec4(0.2));
                break;
        default:finalColor = OriginColor;
    };
}
