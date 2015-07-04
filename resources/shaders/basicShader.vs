#version 120
//#version 330

attribute vec3 position;
attribute vec2 texCoord;

varying vec2 texCoord0;

uniform float texScale;
uniform mat4 modelViewProjection;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	texCoord0 = texCoord * texScale;

}
