#include <opengl/ElementArrayBufferObject.h>

namespace opengl
{
	ElementArrayBufferObject::ElementArrayBufferObject() : BufferObject()
	{
	}
	
	ElementArrayBufferObject::~ElementArrayBufferObject()
	{
	}
	
	void ElementArrayBufferObject::bind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->buffer );
		CheckOpenGLError(glBindBuffer);
	}
	
	void ElementArrayBufferObject::setData( const vector<unsigned short int>& data, Frequency frequency, Access access )
	{
		this->bind();
		this->size = data.size();
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short int) * data.size(), &data[0], BufferObject::getUsage( frequency, access ) );
		CheckOpenGLError(glBufferData);
	}
	
	void ElementArrayBufferObject::draw( OpenGL::DrawMode mode, unsigned int count )
	{
		this->bind();
		glDrawElements( mode, ( count == 0 || count > this->size ? this->size : count), GL_UNSIGNED_SHORT, NULL );
		CheckOpenGLError(glDrawElements);
	}
	
	void ElementArrayBufferObject::draw( unsigned int count )
	{
		this->draw( OpenGL::Triangles, count );
	}
}
