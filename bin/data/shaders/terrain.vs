#version 130

varying vec2 worldPosition;

void main() 
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	worldPosition = (gl_ModelViewMatrix * gl_Vertex).xy;	
    // transform the texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;
}