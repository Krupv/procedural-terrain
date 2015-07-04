#version 120
//#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform float texScale;
uniform mat4 modelViewProjection;
uniform mat4 model;


void main()
{
	texCoord0 = texCoord * texScale;

	vec3 transformedNormal = vec3(model* vec4(normal, 0.0));
	normal0 = normalize(transformedNormal);

	gl_Position = modelViewProjection *  vec4(position, 1.0);

}
