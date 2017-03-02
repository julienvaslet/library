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
			
			int getWidth() const;
			int getHeight() const;
			int getInternalFormat() const;
			int getRedSize() const;
			int getGreenSize() const;
			int getBlueSize() const;
			int getAlphaSize() const;
			int getDepthSize() const;
			int getStencilSize() const;
			
			static void unbind();
	};
}

#endif

