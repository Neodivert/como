/*** 
	Code copied from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

uniform vec4 color;
uniform vec3 ambientLight;

out vec4 finalColor;

void main()
{
	finalColor = vec4( min( color.rgb * ambientLight, vec3( 1.0 ) ), 1.0 );
}

