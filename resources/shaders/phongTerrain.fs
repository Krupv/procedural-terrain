#version 120

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

varying vec2 texCoord0;
varying vec2 detailTexCoord0;
varying vec2 detailTexCoord1;
varying vec3 normal0;

varying float cameraDistance;
varying float height;
varying float slope;

uniform sampler2D terrTexture0;
uniform sampler2D terrTexture1;

uniform vec3 baseColor;
uniform vec3 ambientLight;

uniform DirectionalLight directionalLight;


#define MAX_DETAIL_DISTANCE 800.0
#define MAX_DETAIL_DISTANCE1 400.0
#define MAX_HEIGHT 800.0

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	direction = normalize(direction);
	float diffuseFactor =  max(0.0,  dot(normal, -direction));

	vec4 diffuseColor = vec4(0,0,0,0);

	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(base.color * (base.intensity * diffuseFactor), 1.0);
	}

	return diffuseColor;
}

vec4 calcDirectionalLight(DirectionalLight dirLight, vec3 normal)
{
	return calcLight(dirLight.base, dirLight.direction, normal);
}


void main()
{
	vec4 totalLight = vec4(ambientLight,1);
	vec4 color = vec4(baseColor, 1.0);

	vec4 terrTextureColor0 ;
	vec4 terrTextureColor1 ;
	
	vec4 tex0;

	if ( cameraDistance < MAX_DETAIL_DISTANCE1)
	{

	  float alpha = clamp(cameraDistance / MAX_DETAIL_DISTANCE1, 0.0, 1.0);
	  terrTextureColor0 = mix( texture2D(terrTexture0, detailTexCoord0), texture2D(terrTexture0, detailTexCoord1), 1.0 - alpha);
	  terrTextureColor1 = mix( texture2D(terrTexture1, detailTexCoord0), texture2D(terrTexture1,detailTexCoord1), 1.0 - alpha);
	}	
	else if ( cameraDistance < MAX_DETAIL_DISTANCE)
	{
	  float alpha = clamp( (cameraDistance - MAX_DETAIL_DISTANCE1) / (MAX_DETAIL_DISTANCE-MAX_DETAIL_DISTANCE1), 0.0, 1.0);
	  terrTextureColor0 = mix( texture2D(terrTexture0, texCoord0), texture2D(terrTexture0, detailTexCoord0), 1.0 - alpha);
	  terrTextureColor1 = mix( texture2D(terrTexture1, texCoord0), texture2D(terrTexture1,detailTexCoord0), 1.0 - alpha);
	}
	else
	{

		 terrTextureColor0 = texture2D(terrTexture0, texCoord0);
		 terrTextureColor1 = texture2D(terrTexture1, texCoord0);
	}

	float heightWeight = clamp ( (height / MAX_HEIGHT) - 0.5, 0.0, 1.0);
	tex0  = mix ( terrTextureColor0, terrTextureColor1, heightWeight);

	color *= mix (tex0, terrTextureColor1, slope);

	totalLight += calcDirectionalLight(directionalLight, normal0);

	float FogDensity = 0.0004;
	vec4 fogColor = vec4(0.5,0.5,0.5, 0);
	float dist = gl_FragCoord.z / gl_FragCoord.w;


	//tmp commented
	float fogFactor = (30000 - dist)/(30000 - 10000);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );


	gl_FragColor = mix( fogColor, (color * totalLight), fogFactor);
//	gl_FragColor = vec4 ((color * totalLight).xyz, 0.5);
	//gl_FragColor = color * totalLight;
}
