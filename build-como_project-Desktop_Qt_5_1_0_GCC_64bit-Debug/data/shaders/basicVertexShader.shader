/*** 
	Code copied from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 430 core

in vec3 vPosition;

uniform mat4 modelviewMatrix;
//uniform mat4 projectionMatrix;

void main()
{
	gl_Position = /*projectionMatrix **/ modelViewMatrix * vec4( vPosition.x, vPosition.y, vPosition.z, 1.0f );
}

