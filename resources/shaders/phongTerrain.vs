#version 130
//#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec2 detailTexCoord0;
varying vec2 detailTexCoord1;
varying vec3 normal0;
varying float cameraDistance;
varying float height;
varying float slope;

uniform float texScale;
uniform mat4 modelViewProjection;
uniform mat4 model;

#define DETAIL_SCALE 5.0

void main()
{
	texCoord0 = texCoord * texScale;
	detailTexCoord0 = texCoord0 * DETAIL_SCALE;
	detailTexCoord1 = texCoord0 * DETAIL_SCALE * 5.f;
	//detailTexCoord0 = texCoord0 ;
	//detailTexCoord1 = texCoord0 ;

	normal0 = vec3(model * vec4(normal, 0.0));
	normal0 = normalize(normal0);

	slope = clamp(dot(normal0 , vec3(0,1,0)) + 1.0, 0.0, 1.0);
	//slope = pow(slope, 2);
	//slope = clamp(slope, 0.0, 1.0);
	//slope = (3 * slope * slope  ) - (2*slope*slope*slope);
	//slope = clamp(slope, 0.0, 1.0);

	height = position.y;

	gl_Position = modelViewProjection * vec4(position, 1.0);

	cameraDistance = gl_Position.w;
}
