/*** 
	Code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

uniform vec4 color;
uniform vec3 ambientLight;

const vec3 lightColor = vec3( 0.5f, 0.5f, 0.5f );
const vec3 lightDirection = vec3( 0.0f, -1.0f, 0.0f );
const vec3 halfVector = lightDirection + vec3( 0.0f, 0.0f, 0.0f ); // TODO: H = L + Eye.
const float shininess = 0.5f;
const float strength = 0.5f;

in vec3 Normal;

out vec4 finalColor;

void main()
{
	float diffuse = max( 0.0f, dot( vertexNormal, lightDirection ) );
	float specular = max( 0.0f, dot( vertexNormal, halfVector ) );

	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light
	if( diffuse == 0.0f ){
		specular = 0.0f;
	}else{
		specular = pow( specular, shininess ); // sharpen the highlight
	}

	vec3 scatteredLight = ambientLight + lightColor * diffuse;
	vec3 reflectedLight = lightColor * specular * strength;
	
	// don’t modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min (color.rgb * scatteredLight + reflectedLight, vec3( 1.0f ) );
	finalColor = vec4( rgb, color.a );
	finalColor = vec4( 1.0f );
}

