/*** 
	Code copied from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 430 core

layout(location = 0) in vec3 vPosition;

void main()
{
	gl_Position = vec4( vPosition.x, vPosition.y, vPosition.z, 1.0f );
}

