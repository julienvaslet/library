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
	
	/*bind then glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_.., int* );
	GL_RENDERBUFFER_WIDTH
GL_RENDERBUFFER_HEIGHT
GL_RENDERBUFFER_INTERNAL_FORMAT
GL_RENDERBUFFER_RED_SIZE
GL_RENDERBUFFER_GREEN_SIZE
GL_RENDERBUFFER_BLUE_SIZE
GL_RENDERBUFFER_ALPHA_SIZE
GL_RENDERBUFFER_DEPTH_SIZE
GL_RENDERBUFFER_STENCIL_SIZE
*/
}

