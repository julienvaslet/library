#include <opengl/Framebuffer.h>
#include <opengl/OpenGL.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

using namespace std;

namespace opengl
{
	Framebuffer::Framebuffer() : id(0)
	{
		glGenFramebuffers( 1, &(this->id) );
		CheckOpenGLError(glGenFramebuffers);
		
		#ifdef DEBUG0
		Logger::get() << "[Framebuffer#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers( 1, &(this->id) );
		CheckOpenGLError(glDeleteFramebuffers);
	
		#ifdef DEBUG0
		Logger::get() << "[Framebuffer#" << this->id << "] Deleted." << Logger::endl;
		#endif
	}
	
	unsigned int Framebuffer::getId() const
	{
		return this->id;
	}
	
	void Framebuffer::bind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, this->id );
		CheckOpenGLError(glBindFramebuffer);
	}
	
	void Framebuffer::attachRenderbuffer( Renderbuffer * renderbuffer )
	{
		this->bind();
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer->getId() );
		CheckOpenGLError(glFramebufferRenderbuffer);
	}
	
	void Framebuffer::detachRenderbuffer()
	{
		this->bind();
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0 );
		CheckOpenGLError(glFramebufferRenderbuffer);
	}
	
	bool Framebuffer::isComplete() const
	{
		return glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
	}
	
	void Framebuffer::readPixels( int x, int y, int width, int height, vector<unsigned char> * pixels )
	{
		this->bind();
		
		glReadBuffer( GL_COLOR_ATTACHMENT0 );
		CheckOpenGLError(glReadBuffer);
		glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &((*pixels)[0]) );
		CheckOpenGLError(glReadPixels);
	}
	
	void Framebuffer::unbind()
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		CheckOpenGLError(glBindFramebuffer);
	}
}

