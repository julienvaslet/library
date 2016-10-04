#ifdef GL_ES
#version 300 es
precision mediump float;
#else
#version 130
#endif

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

in vec3 a_Vertex;
out vec4 color;

void main( void )
{
	color = vec4( 1.0, 1.0, 1.0, 1.0 );
	vec4 pos = modelview_matrix * vec4( a_Vertex, 1.0 );
	gl_Position = projection_matrix * pos;
}

