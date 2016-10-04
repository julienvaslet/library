#include <opengl/Shader.h>

#include <sstream>
#include <fstream>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Shader::Shader( Shader::Type type ) : id(0)
	{
		this->id = glCreateShader( static_cast<GLenum>( type ) );
		CheckOpenGLError(glCreateShader);
		
		#ifdef DEBUG0
		Logger::get() << "[" << this->getShaderTypeString() << "#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Shader::~Shader()
	{
		glDeleteShader( this->id );
		CheckOpenGLError(glDeleteShader);
		
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[Shader#" << this->id << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[Shader#" << this->id << "] Could not be marked for deletion." << Logger::endl;
		#endif
	}
	
	string Shader::getShaderTypeString() const
	{
		string value = "";
		Shader::Type type = this->getShaderType();
		
		switch( type )
		{
			case Shader::Vertex:
				value = "VertexShader";
				break;
			
			case Shader::Fragment:
				value = "FragmentShader";
				break;
				
			default:
				value = "Shader";
		}
		
		return value;
	}
	
	bool Shader::getCompileStatus() const
	{
		GLint status;
		glGetShaderiv( this->id, GL_COMPILE_STATUS, &status );
		CheckOpenGLError(glGetShaderiv);
		return status == GL_TRUE;
	}
	
	bool Shader::getDeleteStatus() const
	{
		GLint status;
		glGetShaderiv( this->id, GL_DELETE_STATUS, &status );
		CheckOpenGLError(glGetShaderiv);
		return status == GL_TRUE;
	}

	Shader::Type Shader::getShaderType() const
	{
		GLint type;
		glGetShaderiv( this->id, GL_SHADER_TYPE, &type );
		CheckOpenGLError(glGetShaderiv);
		return static_cast<Shader::Type>( type );
	}
	
	int Shader::getInfoLogLength() const
	{
		GLint length;
		glGetShaderiv( this->id, GL_INFO_LOG_LENGTH, &length );
		CheckOpenGLError(glGetShaderiv);
		return static_cast<int>( length );
	}
	
	int Shader::getShaderSourceLength() const
	{
		GLint length;
		glGetShaderiv( this->id, GL_SHADER_SOURCE_LENGTH, &length );
		CheckOpenGLError(glGetShaderiv);
		return static_cast<int>( length );
	}
	
	string Shader::getInfoLog() const
	{
		int length = this->getInfoLogLength();
		string log( length, ' ' );
		glGetShaderInfoLog( this->id, length, &length, &log[0] );
		CheckOpenGLError(glGetShaderInfoLog);
		return log;
	}
	
	unsigned int Shader::getId() const
	{
		return this->id;
	}
	
	bool Shader::loadFile( const string& filename )
	{
		bool status = false;
		
		stringstream source;
		ifstream shaderFile( filename.c_str() );
		
		if( shaderFile.is_open() )
		{
			source << shaderFile.rdbuf();
			shaderFile.close();
			
			status = this->load( source.str() );
		}
		#ifdef DEBUG1
		else
			Logger::get() << "[" << this->getShaderTypeString() << "#" << this->id << "] Unable to open file \"" << filename << "\"." << Logger::endl;
		#endif
			
		return status;
	}
	
	bool Shader::load( const string& source )
	{
		const char * src = source.c_str();
		glShaderSource( this->id, 1, &src, NULL );
		CheckOpenGLError(glShaderSource);
		glCompileShader( this->id );
		CheckOpenGLError(glCompileShader);

		#ifdef DEBUG1
		if( this->getCompileStatus() )
			Logger::get() << "[" << this->getShaderTypeString() << "#" << this->id << "] Successfuly compiled." << Logger::endl;
		else
			Logger::get() << "[" << this->getShaderTypeString() << "#" << this->id << "] Compilation error: " << this->getInfoLog() << Logger::endl;
		#endif
		
		return this->getCompileStatus();
	}
}
