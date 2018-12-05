#version 130

uniform sampler2D diffuseTexture;

varying vec4 diffuseColor;

void main() 
{
    gl_FragColor = diffuseColor * texture2D(diffuseTexture, gl_TexCoord[0].xy);
}