#include <opengl/Program.h>
#include <opengl/OpenGL.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Program::Program() : id(0)
	{
		this->id = glCreateProgram();
		CheckOpenGLError(glCreateProgram);
		
		#ifdef DEBUG0
		Logger::get() << "[Program#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Program::~Program()
	{
		glDeleteProgram( this->id );
		CheckOpenGLError(glDeteleProgram);
	
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[Program#" << this->id << "] Flagged for deletion." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Deleted." << Logger::endl;
		#endif
	}
	
	bool Program::getLinkStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_LINK_STATUS, &status );
		CheckOpenGLError(glGetProgramiv);
		return status == GL_TRUE;
	}
	
	bool Program::getDeleteStatus() const
	{
		GLint status = GL_FALSE;
		
		if( glIsProgram( this->id ) )
		{
			glGetProgramiv( this->id, GL_DELETE_STATUS, &status );
			CheckOpenGLError(glGetProgramiv);
		}
		
		return status == GL_TRUE;
	}
	
	bool Program::getValidateStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_VALIDATE_STATUS, &status );
		CheckOpenGLError(glGetProgramiv);
		return status == GL_TRUE;
	}
	
	int Program::getInfoLogLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_INFO_LOG_LENGTH, &length );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( length );
	}
	
	int Program::getAttachedShaders() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ATTACHED_SHADERS, &count );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( count );
	}
	
	int Program::getActiveAttributes() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ACTIVE_ATTRIBUTES, &count );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( count );
	}
	
	int Program::getActiveAttributeMaxLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( length );
	}
	
	int Program::getActiveUniforms() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ACTIVE_UNIFORMS, &count );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( count );
	}
	
	int Program::getActiveUniformMaxLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length );
		CheckOpenGLError(glGetProgramiv);
		return static_cast<int>( length );
	}
	
	string Program::getInfoLog() const
	{
		int length = this->getInfoLogLength();
		string log( length, ' ' );
		glGetProgramInfoLog( this->id, length, &length, &log[0] );
		CheckOpenGLError(glGetProgramInfoLog);
		return log;
	}
	
	unsigned int Program::getId() const
	{
		return this->id;
	}
	
	bool Program::loadVertexShaderFile( const string& filename )
	{
		bool status = false;
		
		#ifdef DEBUG0
		Logger::get() << "[Program#" << this->id << "] Loading vertex shader \"" << filename << "\"." << Logger::endl;
		#endif
		
		VertexShader * shader = new VertexShader();
		if( shader->loadFile( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadFragmentShaderFile( const string& filename )
	{
		bool status = false;
		
		#ifdef DEBUG0
		Logger::get() << "[Program#" << this->id << "] Loading fragment shader \"" << filename << "\"." << Logger::endl;
		#endif
		
		FragmentShader * shader = new FragmentShader();
		if( shader->loadFile( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadVertexShader( const string& source )
	{
		bool status = false;
		
		VertexShader * shader = new VertexShader();
		if( shader->load( source ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadFragmentShader( const string& source )
	{
		bool status = false;
		
		FragmentShader * shader = new FragmentShader();
		if( shader->load( source ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::attachShader( Shader * shader, bool deleteShader )
	{
		bool status = true;
		glAttachShader( this->id, shader->getId() );
		
		#ifdef DEBUG0
		status = CheckOpenGLError(glAttachShader);
		#else
		status = (OpenGL::getError() == GL_NO_ERROR);
		#endif
		
		#ifdef DEBUG0
		if( status )
			Logger::get() << "[Program#" << this->id << "] Shader#" << shader->getId() << " attached." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Could not attach Shader#" << shader->getId() << "." << Logger::endl;
		#endif
		
		if( deleteShader )
			delete shader;
			
		return status;
	}
	
	bool Program::link( bool cacheLocations )
	{
		glLinkProgram( this->id );
		
		#ifdef DEBUG1
		if( this->getLinkStatus() )
			Logger::get() << "[Program#" << this->id << "] Successfuly linked." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Linking error: " << this->getInfoLog() << Logger::endl;
		#endif
		
		if( cacheLocations && this->getLinkStatus() )
		{
			// Uniforms
			int count = this->getActiveUniforms();
			int maxlength = this->getActiveUniformMaxLength();
			string name( maxlength, '\0' );
			
			for( int i = 0 ; i < count ; i++ )
			{
				int length = 0;
				GLsizei size = 0;
				GLenum type = GL_FLOAT;
				glGetActiveUniform( this->id, i, name.capacity(), &length, &size, &type, &(name[0]) );
				
				#ifdef DEBUG0
				if( CheckOpenGLError(glGetActiveUniform) )
				#else
				if( OpenGL::getError() == GL_NO_ERROR )
				#endif
					this->getUniformLocation( name.c_str() );
			}
			
			// Attributes
			count = this->getActiveAttributes();
			maxlength = this->getActiveAttributeMaxLength();
			name.reserve( maxlength );
			
			for( int i = 0 ; i < count ; i++ )
			{
				int length = 0;
				GLsizei size = 0;
				GLenum type = GL_FLOAT;
				glGetActiveAttrib( this->id, i, name.capacity(), &length, &size, &type, &(name[0]) );
				
				#ifdef DEBUG0
				if( CheckOpenGLError(glGetActiveAttrib) )
				#else
				if( OpenGL::getError() == GL_NO_ERROR )
				#endif
					this->getAttributeLocation( name.c_str() );
			}
		}
		
		return this->getLinkStatus();
	}

	void Program::bindAttribute( const string& name, unsigned int index )
	{
		glBindAttribLocation( this->id, index, name.c_str() );
		
		#ifdef DEBUG0
		if( CheckOpenGLError(glBindAttribLocation) )
		#else
		if( OpenGL::getError() == GL_NO_ERROR )
		#endif
		{
			this->attributes[name] = index;
			
			#ifdef DEBUG0
			Logger::get() << "[Program#" << this->id << "] Attribute \"" << name << "\" bound at location " << index << "." << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Program#" << this->id << "] Could not bind attribute \"" << name << "\" at index " << index << "." << Logger::endl;
		#endif
	}
	
	void Program::enableAttribute( const string& name )
	{
		glEnableVertexAttribArray( this->getAttributeLocation( name ) );
		CheckOpenGLError(glEnableVertexAttribArray);
	}
	
	void Program::disableAttribute( const string& name )
	{
		glDisableVertexAttribArray( this->getAttributeLocation( name ) );
		CheckOpenGLError(glDisableVertexAttribArray);
	}
	
	int Program::getAttributeLocation( const string& name )
	{
		int location = -1;
		
		map<string, int>::const_iterator it = this->attributes.find( name );
		
		if( it != this->attributes.end() )
			location = it->second;
		else
		{
			location = glGetAttribLocation( this->id, name.c_str() );
			CheckOpenGLError(glGetAttribLocation);
			
			if( location != -1 )
			{
				this->attributes[name] = location;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Caching attribute \"" << name << "\"'s location (" << location << ")." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Program#" << this->id << "] Attribute \"" << name << "\" not found." << Logger::endl;
			}
			#endif
		}
			
		return location;
	}
	
	int Program::getUniformLocation( const string& name )
	{
		int location = -1;
		
		map<string, int>::const_iterator it = this->uniforms.find( name );
		
		if( it != this->uniforms.end() )
			location = it->second;
		else
		{
			location = glGetUniformLocation( this->id, name.c_str() );
			CheckOpenGLError(glGetUniformLocation);
			
			if( location != -1 )
			{
				this->uniforms[name] = location;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Caching uniform \"" << name << "\"'s location (" << location << ")." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Program#" << this->id << "] Uniform \"" << name << "\" not found." << Logger::endl;
			}
			#endif
		}
			
		return location;
	}
	
	void Program::use( bool enableAttributes )
	{
		glUseProgram( this->id );
		CheckOpenGLError(glUseProgram);
		
		if( enableAttributes )
		{
			for( map<string, int>::iterator it = this->attributes.begin() ; it != this->attributes.end() ; it++ )
				this->enableAttribute( it->first );
		}
	}
			
	void Program::sendUniform( const string& uniform, float v0 )
	{
		glUniform1fv( this->getUniformLocation( uniform ), 1, &v0 );
		CheckOpenGLError(glUniform1fv);
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1 )
	{
		float v[2] = { v0, v1 };
		glUniform2fv( this->getUniformLocation( uniform ), 1, v );
		CheckOpenGLError(glUniform2fv);
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1, float v2 )
	{
		float v[3] = { v0, v1, v2 };
		glUniform3fv( this->getUniformLocation( uniform ), 1, v );
		CheckOpenGLError(glUniform3fv);
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1, float v2, float v3 )
	{
		float v[4] = { v0, v1, v2, v3 };
		glUniform4fv( this->getUniformLocation( uniform ), 1, v );
		CheckOpenGLError(glUniform4fv);
	}
	
	void Program::sendUniform( const string& uniform, float * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1fv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform1fv);
				break;
				
			case 2:
				glUniform2fv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform2fv);
				break;
			
			case 3:
				glUniform3fv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform3fv);
				break;

			case 4:		
				glUniform4fv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform4fv);
				break;
		}
	}
	
	void Program::sendUniform( const string& uniform, int v0 )
	{
		glUniform1iv( this->getUniformLocation( uniform ), 1, &v0 );
		CheckOpenGLError(glUniform1iv);
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1 )
	{
		int v[2] = { v0, v1 };
		glUniform2iv( this->getUniformLocation( uniform ), 1 , v );
		CheckOpenGLError(glUniform2iv);
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1, int v2 )
	{
		int v[3] = { v0, v1, v2 };
		glUniform3iv( this->getUniformLocation( uniform ), 1, v );
		CheckOpenGLError(glUniform3iv);
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1, int v2, int v3 )
	{
		int v[4] = { v0, v1, v2, v3 };
		glUniform4iv( this->getUniformLocation( uniform ), 1, v );
		CheckOpenGLError(glUniform4iv);
	}
	
	void Program::sendUniform( const string& uniform, int * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1iv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform1iv);
				break;
				
			case 2:
				glUniform2iv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform2iv);
				break;
			
			case 3:
				glUniform3iv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform3iv);
				break;

			case 4:		
				glUniform4iv( this->getUniformLocation( uniform ), count, values );
				CheckOpenGLError(glUniform4iv);
				break;
		}
	}
	
	void Program::sendUniform( const string& uniform, const Matrix& matrix, bool transpose )
	{
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, transpose, matrix.get() );
		CheckOpenGLError(glUniformMatrix4fv);
	}
	
#ifndef __PI__
	void Program::sendUniform( const string& uniform, unsigned int v0 )
	{
		glUniform1ui( this->getUniformLocation( uniform ), v0 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1 )
	{
		glUniform2ui( this->getUniformLocation( uniform ), v0, v1 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2 )
	{
		glUniform3ui( this->getUniformLocation( uniform ), v0, v1, v2 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3 )
	{
		glUniform4ui( this->getUniformLocation( uniform ), v0, v1, v2, v3 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1uiv( this->getUniformLocation( uniform ), count, values );
				break;
				
			case 2:
				glUniform2uiv( this->getUniformLocation( uniform ), count, values );
				break;
			
			case 3:
				glUniform3uiv( this->getUniformLocation( uniform ), count, values );
				break;

			case 4:		
				glUniform4uiv( this->getUniformLocation( uniform ), count, values );
				break;
		}
	}
	
	void Program::sendModelviewMatrix( const string& uniform )
	{
		float matrix[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, matrix );
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, GL_FALSE, matrix );
	}
	
	void Program::sendProjectionMatrix( const string& uniform )
	{
		float matrix[16];
		glGetFloatv( GL_PROJECTION_MATRIX, matrix );
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, GL_FALSE, matrix );
	}
#endif
		
	void Program::sendUniform( const string& uniform, const Texture& texture, unsigned int unit )
	{
		texture.bind( unit );
		this->sendUniform( uniform, static_cast<int>( unit ) );
	}
		
	void Program::sendAttributePointer( const string& name, ArrayBufferObject * buffer, unsigned int size, unsigned int stride )
	{
		buffer->bind();
		glVertexAttribPointer( this->getAttributeLocation( name ), size, GL_FLOAT, GL_FALSE, stride, 0 );
		CheckOpenGLError(glVertexAttribPointer);
	}
	
	void Program::sendVertexPointer( const string& name, ArrayBufferObject * vbo, unsigned int stride )
	{
		this->sendAttributePointer( name, vbo, 3, stride );
	}
	
	void Program::sendColorPointer( const string& name, ArrayBufferObject * cbo, unsigned int stride )
	{
		this->sendAttributePointer( name, cbo, 4, stride );
	}
	
	void Program::sendTextureCoordinatesPointer( const string& name, ArrayBufferObject * tbo, unsigned int stride )
	{
		this->sendAttributePointer( name, tbo, 2, stride );
	}
}

