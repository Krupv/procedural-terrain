#version 120
//#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec2 detailTexCoord0;
varying float cameraDistance;
varying float height;
varying float slope;

uniform float texScale;
uniform mat4 modelViewProjection;
uniform mat4 model;


#define DETAIL_SCALE 10.0

void main()
{
	texCoord0 = texCoord * texScale;
	detailTexCoord0 = texCoord0 * DETAIL_SCALE;

	vec3 normal0 = vec3(normalize(model  * vec4(normal, 0)));
	slope = dot(normal0 , vec3(0,1,0));

	height = position.y;

	gl_Position = modelViewProjection * vec4(position, 1.0);

	cameraDistance = gl_Position.w;

}
