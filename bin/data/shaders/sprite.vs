#version 130

varying vec2 worldPosition;

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	worldPosition = (gl_ModelViewMatrix * gl_Vertex).xy;	
    gl_TexCoord[0] = gl_MultiTexCoord0;	
	gl_TexCoord[1] = gl_MultiTexCoord1;	
    // forward the vertex color
    gl_FrontColor = gl_Color;
}