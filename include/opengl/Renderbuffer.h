#ifndef __OPENGL_RENDERBUFFER_H
#define __OPENGL_RENDERBUFFER_H	1

#include <opengl/OpenGL.h>

using namespace std;

namespace opengl
{
	class Renderbuffer
	{
		protected:
			GLuint id;
			
		public:
			Renderbuffer( int width, int height );
			virtual ~Renderbuffer();
			
			unsigned int getId() const;
			void bind() const;
			
			static void unbind();
	};
}

#endif

