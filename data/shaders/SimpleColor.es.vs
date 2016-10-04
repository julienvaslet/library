#version 100
precision mediump float;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec3 a_Vertex;
attribute vec3 a_Color;
varying vec4 color;

void main( void )
{
	color = vec4( a_Color, 1.0 );
	gl_Position = projection_matrix * modelview_matrix * vec4( a_Vertex, 1.0 );
}

