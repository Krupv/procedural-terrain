#version 120

varying vec2 texCoord0;
varying vec2 detailTexCoord0;
varying float cameraDistance;
varying float height;
varying float slope;

uniform sampler2D terrTexture0;
uniform sampler2D terrTexture1;

uniform vec3 baseColor;

#define MAX_DETAIL_DISTANCE 300.0
#define MAX_HEIGHT 1000.0


void main()
{
	vec4 color = vec4(baseColor, 1.0);

	vec4 terrTextureColor0 =  texture2D(terrTexture0,  texCoord0);
	vec4 terrTextureColor1 =  texture2D(terrTexture1,  texCoord0);
	
	vec4 tex0;

	if (cameraDistance < MAX_DETAIL_DISTANCE)
	{
		float alpha = cameraDistance / MAX_DETAIL_DISTANCE;
		terrTextureColor0 = mix( texture2D(terrTexture0, detailTexCoord0), terrTextureColor0, alpha);
		terrTextureColor1 = mix( texture2D(terrTexture1, detailTexCoord0), terrTextureColor1, alpha);
	}

	float heightWeight = clamp ( (height / MAX_HEIGHT), 0.0, 1.0);
	tex0 = mix ( terrTextureColor0, terrTextureColor1, heightWeight);

	color *= mix (terrTextureColor1,tex0,  slope);

	gl_FragColor = color; 
}