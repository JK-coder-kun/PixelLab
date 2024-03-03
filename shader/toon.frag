
#version 330 core
out vec4 finalColor;

//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float greyIntensity;
uniform float pixelBlocks;


void main()
{
    float uMagTol =2+ pixelBlocks*5;//293
    float uQuantize = 1+greyIntensity*10;//228
    vec3 rgb = texture(texture1,TexCoord).rgb;
    float mag = sqrt(TexCoord.x*TexCoord.x + TexCoord.y*TexCoord.y);//sqrt( h*h + v*v );
    if( mag > uMagTol )
    {
        gl_FragColor= vec4( 0., 0., 0., 1. );
    }
    else
    {
        rgb.rgb *= uQuantize; // scale up
        rgb.rgb += vec3( .5, .5, .5 ); // round
        ivec3 irgb = ivec3( rgb.rgb ); // cast to all integers
        rgb.rgb = vec3( irgb ); // cast back to floats
        rgb /= uQuantize; // scale down
        gl_FragColor= vec4( rgb, 1. );
    }
}


/*
vec2 lightDir= vec2(.5,.5);
    //float intensity=0.5;
	vec4 color;
	float intensity = dot(lightDir,TexCoord);

	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	gl_FragColor = texture(texture1,TexCoord);
*/
