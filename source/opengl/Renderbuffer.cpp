#include <opengl/Renderbuffer.h>
#include <opengl/OpenGL.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

using namespace std;

namespace opengl
{
	Renderbuffer::Renderbuffer( int width, int height )
	{
		glGenRenderbuffers( 1, &(this->id) );
		CheckOpenGLError(glGenRenderbuffers);
		
		#ifdef DEBUG0
		Logger::get() << "[Renderbuffer#" << this->id << "] Created." << Logger::endl;
		#endif
		
		this->bind();
		//glRenderbufferStorageMultisample( GL_RENDERBUFFER, 4, GL_RGBA, width, height );
		//CheckOpenGLError(glRenderbufferStorageMultisample);
		glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA, width, height );
		CheckOpenGLError(glRenderbufferStorage);
	}
	
	Renderbuffer::~Renderbuffer()
	{
		glDeleteRenderbuffers( 1, &(this->id) );
		CheckOpenGLError(glDeleteRenderbuffers);
	
		#ifdef DEBUG0
		Logger::get() << "[Renderbuffer#" << this->id << "] Deleted." << Logger::endl;
		#endif
	}
	
	unsigned int Renderbuffer::getId() const
	{
		return this->id;
	}
	
	void Renderbuffer::bind() const
	{
		glBindRenderbuffer( GL_RENDERBUFFER, this->id );
		CheckOpenGLError(glBindRenderbuffer);
	}
	
	void Renderbuffer::unbind()
	{
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		CheckOpenGLError(glBindRenderbuffer);
	}
	
	int Renderbuffer::getWidth() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getHeight() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getInternalFormat() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getRedSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_RED_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	int 
	Renderbuffer::getGreenSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_GREEN_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getBlueSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_BLUE_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getAlphaSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_ALPHA_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getDepthSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
	
	int Renderbuffer::getStencilSize() const
	{
		this->bind();
		int value = 0;
		
		glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &value );
		CheckOpenGLError(glGetRenderbufferParameteriv);
		
		return value;
	}
}

