#ifdef GL_ES
#version 300 es
precision mediump float;
#else
#version 130
#endif

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

in vec3 a_Vertex;
in vec2 a_TexCoord0;
out vec2 texCoord0;

void main( void )
{
	texCoord0 = a_TexCoord0;
	vec4 pos = modelview_matrix * vec4( a_Vertex, 1.0 );
	gl_Position = projection_matrix * pos;
}

