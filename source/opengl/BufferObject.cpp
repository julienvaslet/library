#include <opengl/BufferObject.h>

#ifdef DEBUG0
#include <iostream>
using namespace std;
#endif

namespace opengl
{
	BufferObject::BufferObject() : buffer(0)
	{
		glGenBuffers( 1, &(this->buffer ) );
		CheckOpenGLError(glGenBuffers);
		
		#ifdef DEBUG0
		cout << "[BufferObject#" << this->buffer << "] Generated." << endl;
		#endif
	}
	
	BufferObject::~BufferObject()
	{
		if( glIsBuffer( this->buffer ) == GL_TRUE )
		{
			glDeleteBuffers( 1, &(this->buffer) );
			CheckOpenGLError(glDeleteBuffers);
		}
		
		#ifdef DEBUG0
		cout << "[BufferObject#" << this->buffer << "] Deleted." << endl;
		#endif
	}
	
	GLuint BufferObject::getBuffer()
	{
		return this->buffer;
	}
	
	GLenum BufferObject::getUsage( Frequency f, Access a )
	{
#ifdef __PI__
		GLenum usages[] = { GL_STREAM_DRAW, GL_STREAM_DRAW, GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_DYNAMIC_DRAW, GL_DYNAMIC_DRAW };
#else
		GLenum usages[] = { GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY };
#endif
		return usages[ static_cast<int>( f ) + static_cast<int>( a ) ];
	}
}
