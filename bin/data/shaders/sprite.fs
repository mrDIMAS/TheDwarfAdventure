// If you can optimize this mess, mail me to d1maxa@yandex.ru
// any suggestions is appreciated

#version 130

uniform sampler2D diffuseTexture;
uniform sampler2D decalTexture;

#define MAX_LIGHTS 8

uniform int lightCount;
uniform vec4 ambientColor;
uniform vec4 lightPosRadius[MAX_LIGHTS]; // xy - position, z - radius
uniform vec4 lightColor[MAX_LIGHTS]; // xyz - color
uniform bool lightingEnabled;
uniform float topBackgroundlessPartHeight;

varying vec2 worldPosition;

void main()
 {
    vec4 diffuse = texture2D(diffuseTexture, gl_TexCoord[0].xy);
					
	// decal mixing
	vec4 decal = texture2D(decalTexture, gl_TexCoord[1].xy);
	vec4 mixed = vec4(diffuse.rgb * (1.0 - decal.a) + decal.rgb * decal.a, diffuse.a);	
	vec4 outColor = gl_Color * mixed;
	
	// lighting
	if(worldPosition.y > topBackgroundlessPartHeight && lightingEnabled) 
	{		
		vec4 pixelLighting = vec4(0, 0, 0, 1);
		for(int i = 0; i < lightCount; ++i) 
		{
			float distance = length( worldPosition - lightPosRadius[i].xy );
			float kD = distance / lightPosRadius[i].z;
			float attenuation = 1 - min( 1, kD * kD );
			pixelLighting.xyz += lightColor[i].xyz * attenuation;
		}
		outColor *= ambientColor + pixelLighting;
	}
	
	// final color
	gl_FragColor = outColor;
}