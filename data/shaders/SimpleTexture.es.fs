#version 100
precision mediump float;

uniform sampler2D texture0;
varying vec2 texCoord0;

void main(void)
{
	gl_FragColor = texture2D( texture0, texCoord0 );
}

