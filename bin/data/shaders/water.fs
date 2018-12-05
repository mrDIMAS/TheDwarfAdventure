#version 130

uniform sampler2D diffuseTexture;
uniform vec2 dxdy;

varying vec2 texCoord;

void main()
{
	// gaussian blur
	vec4 outColor = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
	outColor += texture2D(diffuseTexture, texCoord - 3 * dxdy );
    outColor += texture2D(diffuseTexture, texCoord - 2 * dxdy );
    outColor += texture2D(diffuseTexture, texCoord - dxdy );
    outColor += texture2D(diffuseTexture, texCoord );
    outColor += texture2D(diffuseTexture, texCoord + dxdy );
    outColor += texture2D(diffuseTexture, texCoord + 2 * dxdy );	
	outColor += texture2D(diffuseTexture, texCoord + 3 * dxdy );	
	gl_FragColor = outColor * 0.2;
}