/*** 
	Code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

uniform vec4 color;

// Lights.
struct Light {
	vec3 color;
};
uniform Light lights[2];

// Directional light.
struct DirectionalLight {
	unsigned int lightIndex;
	vec3 lightVector;
	vec3 halfVector;
} directionalLight;

const vec3 lightDirection = vec3( 0.0f, -1.0f, 0.0f );
const vec3 halfVector = lightDirection + vec3( 0.0f, 0.0f, 0.0f ); // TODO: H = L + Eye (or L - Eye?).
const float shininess = 0.9f;
const float strength = 0.9f;

in vec3 normal;

out vec4 finalColor;

void main()
{
	float diffuse = max( 0.0f, dot( normal, lightDirection ) );
	float specular = max( 0.0f, dot( normal, halfVector ) );

	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light
	if( diffuse == 0.0f ){
		specular = 0.0f;
	}else{
		specular = pow( specular, shininess ); // sharpen the highlight
	}

	vec3 scatteredLight = ambientLight + lights[0].color * diffuse;
	vec3 reflectedLight = lights[0].color * specular * strength;
	
	// don’t modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min (color.rgb * scatteredLight + reflectedLight, vec3( 1.0f ) );
	finalColor = vec4( rgb, color.a );
}

