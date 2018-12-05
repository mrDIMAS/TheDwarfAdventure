#version 130

uniform sampler2D diffuseTexture;

void main() 
{	
	vec4 outColor = texture2D(diffuseTexture, gl_TexCoord[0].xy);
	if(outColor.a < 0.16)
	{
		discard;
	}
	gl_FragColor = outColor;
}