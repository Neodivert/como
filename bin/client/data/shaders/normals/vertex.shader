/*** 
	Base code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

out VS_GS_INTERFACE {
	vec4 pos;
	vec3 normal;
} outVertex;

void main()
{
	// Transform vertex position and normal.
	outVertex.pos = mvpMatrix * vec4( vPosition.xyz, 1.0f );
	gl_Position = outVertex.pos;
	outVertex.normal = normalize( normalMatrix * vNormal );
}

