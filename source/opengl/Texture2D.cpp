#include <opengl/Texture2D.h>
#include <tools/logger/Logger.h>

using namespace tools::logger;

namespace opengl
{
	Texture2D::Texture2D() : Texture()
	{
		this->bind();
		this->setWrapMode( GL_REPEAT );
		this->setFiltering( GL_NEAREST, GL_NEAREST );
	}
	
	Texture2D::~Texture2D()
	{
	}
	
	unsigned int Texture2D::getWidth()
	{
		return this->width;
	}
	
	unsigned int Texture2D::getHeight()
	{
		return this->height;
	}
	
	void Texture2D::bind( unsigned int unit ) const
	{
		glActiveTexture( GL_TEXTURE0 + unit );
		CheckOpenGLError(glActiveTexture);
		glBindTexture( GL_TEXTURE_2D, this->id );
		CheckOpenGLError(glBindTexture);
	}
	
	void Texture2D::setData( const Image& image )
	{
		this->setData( image.getData(), image.getWidth(), image.getHeight(), image.getBytesPerPixel() == 4 ? GL_RGBA : GL_RGB );
	}
	
	void Texture2D::setData( const unsigned char * data, unsigned int width, unsigned int height, GLenum format )
	{
		this->bind();
		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		CheckOpenGLError(glTexImage2D);
		
		this->width = width;
		this->height = height;
	}
	
	void Texture2D::setWrapMode( GLenum mode )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode );
		CheckOpenGLError(glTexParameteri);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode );
		CheckOpenGLError(glTexParameteri);
	}
	
	void Texture2D::setFiltering( GLenum minification, GLenum magnification )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification );
		CheckOpenGLError(glTexParameteri);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification );
		CheckOpenGLError(glTexParameteri);
	}
}

