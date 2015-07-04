#version 120

varying vec2 texCoord0;

uniform sampler2D diffuse;

uniform vec3 baseColor;

varying float height;

void main()
{
	vec4 color = vec4(baseColor, 1.0);
	vec4 textureColor = texture2D(diffuse, texCoord0);

	if (textureColor != vec4(0,0,0,0))
			color *= textureColor;

	gl_FragColor = color; 
}
