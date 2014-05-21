/*** 
	Code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

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
};
uniform DirectionalLight directionalLight;

// Material info
struct Material {
	vec4 color;
	vec3 ambientReflexivity;
	vec3 diffuseReflectivity;
	vec3 specularReflectivity;
	float specularExponent;
};
uniform Material material;

in vec3 normal;

out vec4 finalColor;

void main()
{
/*
	vec3 halfVector = directionalLight.lightVector + vec3( 0.0f, 0.0f, 0.0f ); // TODO: H = L + Eye (or L - Eye?).

	float diffuse = max( 0.0f, dot( normal, directionalLight.lightVector ) );
	float specular = max( 0.0f, dot( normal, halfVector ) );

	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light
	if( diffuse == 0.0f ){
		specular = 0.0f;
	}else{
		specular = pow( specular, material.specularExponent ); // sharpen the highlight
	}

	vec3 scatteredLight = lights[0].color * diffuse;
	vec3 reflectedLight = lights[0].color * specular * material.specularReflectivity;
	
	// don’t modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min (color.rgb * scatteredLight + reflectedLight, vec3( 1.0f ) );
	finalColor = vec4( rgb, color.a );
*/
	finalColor = material.color;
}

