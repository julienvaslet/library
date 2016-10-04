#ifndef __OPENGL_FRAGMENTSHADER_H
#define __OPENGL_FRAGMENTSHADER_H	1

#include <opengl/Shader.h>

#ifdef __PI__
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

using namespace std;

namespace opengl
{
	class FragmentShader : public Shader
	{
		protected:
			
		public:
			FragmentShader();
			virtual ~FragmentShader();
			
			virtual Shader::Type getShaderType() const;
	};
}

#endif

