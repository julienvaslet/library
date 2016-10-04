#version 100
precision mediump float;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec3 a_Vertex;
attribute vec2 a_TexCoord0;
varying vec2 texCoord0;

void main( void )
{
	texCoord0 = a_TexCoord0;
	gl_Position = projection_matrix * modelview_matrix * vec4( a_Vertex, 1.0 );
}

