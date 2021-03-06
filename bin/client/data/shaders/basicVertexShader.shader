/*** 
	Base code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 inUVCoordinates;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

out vec3 normal;
out vec2 uvCoordinates;

void main()
{
	// Transform vertex position and normal.
	gl_Position = mvpMatrix * vec4( vPosition.xyz, 1.0f );
	normal = normalize( normalMatrix * vNormal );

	uvCoordinates = inUVCoordinates;
}

