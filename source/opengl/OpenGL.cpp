#include <opengl/OpenGL.h>
#include <SDL2/SDL.h>
#include <tools/logger/Logger.h>

#include <sstream>

using namespace std;
using namespace tools::logger;

namespace opengl
{
	string OpenGL::vendor;
	string OpenGL::renderer;
	string OpenGL::version;
	set<string> OpenGL::extensions;
	
	OpenGL::OpenGL()
	{
	}
	
	OpenGL::~OpenGL()
	{
	}
	
	const string& OpenGL::getVendor()
	{
		if( OpenGL::vendor.length() == 0 )
			OpenGL::vendor = (const char *) glGetString( GL_VENDOR );
			
		return OpenGL::vendor;
	}
	
	const string& OpenGL::getRenderer()
	{
		if( OpenGL::renderer.length() == 0 )
			OpenGL::renderer = (const char *) glGetString( GL_RENDERER );
			
		return OpenGL::renderer;
	}
	
	const string& OpenGL::getVersion()
	{
		if( OpenGL::version.length() == 0 )
			OpenGL::version = (const char *) glGetString( GL_VERSION );
			
		return OpenGL::version;
	}
	
	const set<string>& OpenGL::getExtensions()
	{
		if( OpenGL::extensions.size() == 0 )
		{
			const char * exts = (const char *) glGetString( GL_EXTENSIONS );
		
			unsigned int i = 0;
			stringstream extension;
		
			while( exts[i] != 0 )
			{
				if( exts[i] == ' ' )
				{
					OpenGL::extensions.insert( extension.str() );
					extension.str( "" );
				}
				else
					extension << exts[i];
			
				i++;
			}
		}
		
		return OpenGL::extensions;
	}
		
	bool OpenGL::hasExtension( const string& extension )
	{
		set<string>::const_iterator it = OpenGL::getExtensions().find( extension );
		return it != OpenGL::getExtensions().end();
	}
	
	GLenum OpenGL::getError()
	{
		return glGetError();
	}
	
	bool OpenGL::checkError( const char * function, const char * file, unsigned int line )
	{
		bool status = true;
		GLenum error = OpenGL::getError();
		
		if( error != GL_NO_ERROR )
		{
			status = false;
			Logger::get() << "[OpenGL][Error#" << error << "]";
			
			if( function != NULL )
				Logger::get() << "[" << function << "]";
			
			Logger::get() << " ";
			
			switch( error )
			{
				case GL_INVALID_OPERATION:
					Logger::get() << "GL_INVALID_OPERATION";
					break;
				
				case GL_INVALID_ENUM:
					Logger::get() << "GL_INVALID_ENUM";
					break;
					
				case GL_INVALID_VALUE:
					Logger::get() << "GL_INVALID_VALUE";
					break;
					
				case GL_OUT_OF_MEMORY:
					Logger::get() << "GL_OUT_OF_MEMORY";
					break;
					
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					Logger::get() << "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
					
				default:
					Logger::get() << "Unknown error";
					break;
			}
			
			if( file != NULL )
			{
				Logger::get() << " (" << file;
				
				if( line > 0 )
					Logger::get() << ", line " << line;
					
				Logger::get() << ")";
			}
			
			Logger::get() << Logger::endl;
		}
		
		return status;
	}

	void OpenGL::initialize()
	{
		#ifdef DEBUG0
		Logger::get() << "[OpenGL] Initialization..." << Logger::endl;
		#endif
		
		// Draw functions
		
		// Not supported in GLES2
		LoadOpenGLFunction_Windows(glDrawRangeElements);
		
		// Textures functions
		LoadOpenGLFunction_Windows(glActiveTexture);
		
		// Buffers functions
#ifndef __PI__
		LoadOpenGLFunction(glGenBuffers);
		LoadOpenGLFunction(glIsBuffer);
		LoadOpenGLFunction(glDeleteBuffers);
		LoadOpenGLFunction(glBindBuffer);
		LoadOpenGLFunction(glBufferData);
#endif
		
		// Shaders functions
#ifndef __PI__
		LoadOpenGLFunction(glCreateShader);
		LoadOpenGLFunction(glDeleteShader);
		LoadOpenGLFunction(glGetShaderiv);
		LoadOpenGLFunction(glGetShaderInfoLog);
		LoadOpenGLFunction(glShaderSource);
		LoadOpenGLFunction(glCompileShader);
#endif

		// Program functions
#ifndef __PI__
		LoadOpenGLFunction(glCreateProgram);
		LoadOpenGLFunction(glDeleteProgram);
		LoadOpenGLFunction(glGetProgramiv);
		LoadOpenGLFunction(glGetProgramInfoLog);
		LoadOpenGLFunction(glAttachShader);
		LoadOpenGLFunction(glLinkProgram);
		LoadOpenGLFunction(glUseProgram);
		LoadOpenGLFunction(glIsProgram);
		LoadOpenGLFunction(glGetActiveAttrib);
		LoadOpenGLFunction(glGetActiveUniform);
		LoadOpenGLFunction(glBindAttribLocation);
		LoadOpenGLFunction(glEnableVertexAttribArray);
		LoadOpenGLFunction(glDisableVertexAttribArray);
		LoadOpenGLFunction(glVertexAttribPointer);
		LoadOpenGLFunction(glGetAttribLocation);
		LoadOpenGLFunction(glGetUniformLocation);
#endif

		// Uniforms functions
#ifndef __PI__
		LoadOpenGLFunction(glUniform1f);
		LoadOpenGLFunction(glUniform2f);
		LoadOpenGLFunction(glUniform3f);
		LoadOpenGLFunction(glUniform4f);
		LoadOpenGLFunction(glUniform1fv);
		LoadOpenGLFunction(glUniform2fv);
		LoadOpenGLFunction(glUniform3fv);
		LoadOpenGLFunction(glUniform4fv);
		LoadOpenGLFunction(glUniform1i);
		LoadOpenGLFunction(glUniform2i);
		LoadOpenGLFunction(glUniform3i);
		LoadOpenGLFunction(glUniform4i);
		LoadOpenGLFunction(glUniform1iv);
		LoadOpenGLFunction(glUniform2iv);
		LoadOpenGLFunction(glUniform3iv);
		LoadOpenGLFunction(glUniform4iv);
		LoadOpenGLFunction(glUniformMatrix4fv);
		
		// Not supported in GLES2
		LoadOpenGLFunction(glUniform1ui);
		LoadOpenGLFunction(glUniform2ui);
		LoadOpenGLFunction(glUniform3ui);
		LoadOpenGLFunction(glUniform4ui);
		LoadOpenGLFunction(glUniform1uiv);
		LoadOpenGLFunction(glUniform2uiv);
		LoadOpenGLFunction(glUniform3uiv);
		LoadOpenGLFunction(glUniform4uiv);
#endif
		
		#ifdef DEBUG1
		Logger::get() << "[OpenGL] Initialized." << Logger::endl;
		#endif
	}
}


// Draw functions
InitializeOpenGLFunction_Windows(glDrawRangeElements);

// Textures functions
InitializeOpenGLFunction_Windows(glActiveTexture);

// Buffers functions
#ifndef __PI__
InitializeOpenGLFunction(glGenBuffers);
InitializeOpenGLFunction(glIsBuffer);
InitializeOpenGLFunction(glDeleteBuffers);
InitializeOpenGLFunction(glBindBuffer);
InitializeOpenGLFunction(glBufferData);
#endif

// Shaders functions
#ifndef __PI__
InitializeOpenGLFunction(glCreateShader);
InitializeOpenGLFunction(glDeleteShader);
InitializeOpenGLFunction(glGetShaderiv);
InitializeOpenGLFunction(glGetShaderInfoLog);
InitializeOpenGLFunction(glShaderSource);
InitializeOpenGLFunction(glCompileShader);
#endif

// Programs functions
#ifndef __PI__
InitializeOpenGLFunction(glCreateProgram);
InitializeOpenGLFunction(glDeleteProgram);
InitializeOpenGLFunction(glGetProgramiv);
InitializeOpenGLFunction(glGetProgramInfoLog);
InitializeOpenGLFunction(glAttachShader);
InitializeOpenGLFunction(glLinkProgram);
InitializeOpenGLFunction(glUseProgram);
InitializeOpenGLFunction(glIsProgram);
InitializeOpenGLFunction(glGetActiveAttrib);
InitializeOpenGLFunction(glGetActiveUniform);
InitializeOpenGLFunction(glBindAttribLocation);
InitializeOpenGLFunction(glEnableVertexAttribArray);
InitializeOpenGLFunction(glDisableVertexAttribArray);
InitializeOpenGLFunction(glVertexAttribPointer);
InitializeOpenGLFunction(glGetAttribLocation);
InitializeOpenGLFunction(glGetUniformLocation);
#endif

// Uniforms functions
#ifndef __PI__
InitializeOpenGLFunction(glUniform1f);
InitializeOpenGLFunction(glUniform2f);
InitializeOpenGLFunction(glUniform3f);
InitializeOpenGLFunction(glUniform4f);
InitializeOpenGLFunction(glUniform1fv);
InitializeOpenGLFunction(glUniform2fv);
InitializeOpenGLFunction(glUniform3fv);
InitializeOpenGLFunction(glUniform4fv);
InitializeOpenGLFunction(glUniform1i);
InitializeOpenGLFunction(glUniform2i);
InitializeOpenGLFunction(glUniform3i);
InitializeOpenGLFunction(glUniform4i);
InitializeOpenGLFunction(glUniform1iv);
InitializeOpenGLFunction(glUniform2iv);
InitializeOpenGLFunction(glUniform3iv);
InitializeOpenGLFunction(glUniform4iv);
InitializeOpenGLFunction(glUniformMatrix4fv);

// Not supported in GLES2
InitializeOpenGLFunction(glUniform1ui);
InitializeOpenGLFunction(glUniform2ui);
InitializeOpenGLFunction(glUniform3ui);
InitializeOpenGLFunction(glUniform4ui);
InitializeOpenGLFunction(glUniform1uiv);
InitializeOpenGLFunction(glUniform2uiv);
InitializeOpenGLFunction(glUniform3uiv);
InitializeOpenGLFunction(glUniform4uiv);
#endif

