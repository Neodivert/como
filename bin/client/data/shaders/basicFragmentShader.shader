/*** 
	Code copied and adapted from the following OpenGL book:
	SHREINER, Dave; GRAHAM, Sellers; Kessenich John; LICEA-KANE, Bill. OpenGL. 
	Programming Guide. Eigth Edition. The Official Guide to Learning OpenGL, 
	Version 4.3. The Khronos OpenGL ARB Working Group . Editorial Pearson. 2013.
***/

#version 420 core

uniform bool lightingEnabled;
uniform bool texturingEnabled;

const unsigned int MAX_DIRECTIONAL_LIGHTS = 4;
const unsigned int MAX_LIGHTS = MAX_DIRECTIONAL_LIGHTS;

// Lights.
struct Light {
	vec3 color;
	float ambientCoefficient;
};
uniform Light lights[MAX_LIGHTS];

// Directional light.
struct DirectionalLight {
	int lightIndex;
	vec3 lightVector;
	vec3 halfVector;
};
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

uniform sampler2D textureSampler;

// Material info
struct Material {
	vec4 color;
	vec3 ambientReflectivity;
	vec3 diffuseReflectivity;
	vec3 specularReflectivity;
	float specularExponent;
};
uniform Material material;

in vec3 normal;
in vec2 uvCoordinates;

out vec4 finalColor;

void main()
{
	finalColor = vec4( 0.0f );
	vec4 meshColor;

	unsigned int i;
	if( lightingEnabled ){
		for( i=0; i<MAX_DIRECTIONAL_LIGHTS; i++ ){
			if( directionalLights[i].lightIndex >= 0 ){
				vec3 halfVector = directionalLights[i].lightVector + vec3( 0.0f, 0.0f, 0.0f ); // TODO: H = L + Eye (or L - Eye?).

				float diffuse = max( 0.0f, dot( normal, directionalLights[i].lightVector ) );
				float specular = max( 0.0f, dot( normal, halfVector ) );

				// surfaces facing away from the light (negative dot products)
				// won’t be lit by the directional light
				if( diffuse == 0.0f ){
					specular = 0.0f;
				}else{
					specular = pow( specular, material.specularExponent ); // sharpen the highlight
				}

				vec3 ambientColor = lights[ directionalLights[i].lightIndex ].ambientCoefficient * lights[ directionalLights[i].lightIndex ].color * material.ambientReflectivity;
				vec3 scatteredLight = ambientColor + lights[ directionalLights[i].lightIndex ].color * diffuse * material.diffuseReflectivity;
				vec3 reflectedLight = lights[ directionalLights[i].lightIndex ].color * specular * material.specularReflectivity;
	
				if( texturingEnabled ){
					meshColor = texture( textureSampler, uvCoordinates );
					meshColor.a = 1.0f;
				}else{
					meshColor = vec4( material.color.rgb, 1.0f );
				}
				// don’t modulate the underlying color with reflected light,
				// only with scattered light
				vec3 rgb = min ( meshColor.rgb * scatteredLight + reflectedLight, vec3( 1.0f ) );
				finalColor += vec4( rgb, meshColor.a );
			}
		}
	}else{
		finalColor = material.color;
	}
}

