#ifndef __OPENGL_VERTEXSHADER_H
#define __OPENGL_VERTEXSHADER_H	1

#include <opengl/Shader.h>

#ifdef __PI__
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

using namespace std;

namespace opengl
{
	class VertexShader : public Shader
	{
		protected:
			
		public:
			VertexShader();
			virtual ~VertexShader();
			
			virtual Shader::Type getShaderType() const;
	};
}

#endif

