#version 130

uniform sampler2D diffuseTexture;
uniform sampler2D backgroundMask;
uniform sampler3D backgroundStack;

#define MAX_LIGHTS 8

uniform int lightCount;
uniform vec4 ambientColor;
uniform vec4 lightPosRadius[MAX_LIGHTS]; // xy - position, z - radius
uniform vec4 lightColor[MAX_LIGHTS]; // xyz - color
uniform float topBackgroundlessPartHeight;
uniform float backgroundStackSize; // = UINT8_MAX / StackSize

varying vec2 worldPosition;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

float SobelEdgeDetection() 
{
	const float d = 1.0 / 256.0;
	
    float diffuse = texture2D(diffuseTexture, gl_TexCoord[0].xy).a;
    	
	mat3 I;
    for(int i = 0; i < 3; i++) 
	{
        for(int j = 0; j < 3; j++)
		{
             I[i][j] = texture2D(diffuseTexture, gl_TexCoord[0].xy + vec2(d * (i - 1), d * (j - 1))).a; 
		}
    }
	
	float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
	float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

	return sqrt(gx * gx + gy * gy);
} 

void main() {
    vec4 diffuse = texture2D(diffuseTexture, gl_TexCoord[0].xy);
	
	float shadowFactor = 1.0f - 0.08f * SobelEdgeDetection();
	
	// if front's layer pixel is invisible
	if(diffuse.a <= 0.5) 
	{
		if(worldPosition.y > topBackgroundlessPartHeight)  
		{
			// then lookup background pixel using background mask with indices of textures
			// inside volume texture
			const float bias = 0.001;
			float layer = texture2D(backgroundMask, gl_TexCoord[0].xy ).r * 255.0 / backgroundStackSize;
			vec3 tc = vec3(gl_TexCoord[0].x * 4, gl_TexCoord[0].y * 4, layer + bias);
			diffuse.xyz = texture3D(backgroundStack, tc).xyz;
			// Zero layer is transparent 
			if(layer > 0) 
			{
				diffuse.a = 1.0;
			} 
			else
			{
				diffuse.a = 0.0;
			}
		}
	}
	
	// lighting
	if(worldPosition.y > topBackgroundlessPartHeight)
	{		
		vec4 pixelLighting = vec4(0, 0, 0, 1);
		for(int i = 0; i < lightCount; ++i)
		{
			float distance = length( worldPosition - lightPosRadius[i].xy );
			float kD = distance / lightPosRadius[i].z;
			float attenuation = 1 - min( 1, kD * kD );
			pixelLighting.xyz += lightColor[i].xyz * attenuation;
		}
		diffuse *= ambientColor + pixelLighting;
	}
	
	diffuse.xyz *= shadowFactor;
	
	// final color
	gl_FragColor = diffuse;
}