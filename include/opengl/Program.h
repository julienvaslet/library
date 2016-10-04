#ifndef __OPENGL_PROGRAM_H
#define __OPENGL_PROGRAM_H	1

#include <opengl/OpenGL.h>
#include <opengl/VertexShader.h>
#include <opengl/FragmentShader.h>
#include <opengl/Matrix.h>
#include <opengl/Texture.h>
#include <opengl/ArrayBufferObject.h>

#include <string>
#include <map>

using namespace std;

namespace opengl
{
	class Program
	{
		protected:
			GLuint id;
			map<string, int> attributes;
			map<string, GLint> uniforms;
			
		public:
			Program();
			virtual ~Program();
			
			bool getLinkStatus() const;
			bool getDeleteStatus() const;
			bool getValidateStatus() const;
			int getInfoLogLength() const;
			int getAttachedShaders() const;
			int getActiveAttributes() const;
			int getActiveAttributeMaxLength() const;
			int getActiveUniforms() const;
			int getActiveUniformMaxLength() const;
			string getInfoLog() const;
			
			unsigned int getId() const;
			bool loadVertexShaderFile( const string& filename );
			bool loadFragmentShaderFile( const string& filename );
			bool loadVertexShader( const string& source );
			bool loadFragmentShader( const string& source );
			
			bool attachShader( Shader * shader, bool deleteShader = false );
			void bindAttribute( const string& name, unsigned int index );
			void enableAttribute( const string& name );
			void disableAttribute( const string& name );
			int getAttributeLocation( const string& name );
			int getUniformLocation( const string& name );
			bool link( bool cacheLocations = false );
			
			void sendUniform( const string& uniform, float v0 );
			void sendUniform( const string& uniform, float v0, float v1 );
			void sendUniform( const string& uniform, float v0, float v1, float v2 );
			void sendUniform( const string& uniform, float v0, float v1, float v2, float v3 );
			void sendUniform( const string& uniform, float * values, unsigned int size, unsigned int count = 1 );
			void sendUniform( const string& uniform, int v0 );
			void sendUniform( const string& uniform, int v0, int v1 );
			void sendUniform( const string& uniform, int v0, int v1, int v2 );
			void sendUniform( const string& uniform, int v0, int v1, int v2, int v3 );
			void sendUniform( const string& uniform, int * values, unsigned int size, unsigned int count = 1 );
			void sendUniform( const string& uniform, const Matrix& matrix, bool transpose = false );
			void sendUniform( const string& uniform, const Texture& texture, unsigned int unit = 0 );

#ifndef __PI__
			void sendUniform( const string& uniform, unsigned int v0 );
			void sendUniform( const string& uniform, unsigned int v0, unsigned int v1 );
			void sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2 );
			void sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3 );
			void sendUniform( const string& uniform, unsigned int * values, unsigned int size, unsigned int count = 1 );
			
			void sendModelviewMatrix( const string& uniform );
			void sendProjectionMatrix( const string& uniform );
#endif
			
			void sendAttributePointer( const string& name, ArrayBufferObject * buffer, unsigned int size, unsigned int stride = 0 );
			void sendVertexPointer( const string& name, ArrayBufferObject * vbo, unsigned int stride = 0 );
			void sendColorPointer( const string& name, ArrayBufferObject * cbo, unsigned int stride = 0 );
			void sendTextureCoordinatesPointer( const string& name, ArrayBufferObject * tbo, unsigned int stride = 0 );
			
			void use( bool enableAttributes = false );
	};
}

#endif

