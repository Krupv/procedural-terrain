#version 120

varying vec2 texCoord0;
varying vec3 normal0;


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



uniform sampler2D diffuse;
uniform vec3 baseColor;
uniform vec3 ambientLight;

uniform DirectionalLight directionalLight;



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
	vec4 texColor = texture2D(diffuse, texCoord0); 

	color *= texColor;

	totalLight += calcDirectionalLight(directionalLight, normal0);

	gl_FragColor = color * totalLight;
}
