#include <opengl/Texture.h>
#include <tools/logger/Logger.h>

using namespace tools::logger;

namespace opengl
{
	Texture::Texture() : id(0)
	{
		glGenTextures( 1, &(this->id) );
		CheckOpenGLError(glGenTextures);
		
		#ifdef DEBUG0
		Logger::get() << "[Texture#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Texture::~Texture()
	{
		if( glIsTexture( this->id ) == GL_TRUE )
		{
			glDeleteTextures( 1, &(this->id) );
			CheckOpenGLError(glDeleteTextures);

			#ifdef DEBUG0
			Logger::get() << "[Texture#" << this->id << "] Deleted." << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
		{
			Logger::get() << "[Texture#" << this->id << "] Could not be deleted, this is not a valid texture." << Logger::endl;
		}
		#endif
	}
	
	unsigned int Texture::getId() const
	{
		return this->id;
	}
}

