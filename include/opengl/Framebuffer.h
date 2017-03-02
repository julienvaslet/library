#ifndef __OPENGL_FRAMEBUFFER_H
#define __OPENGL_FRAMEBUFFER_H	1

#include <opengl/OpenGL.h>
#include <opengl/Renderbuffer.h>
#include <vector>

using namespace std;

namespace opengl
{
	class Framebuffer
	{
		protected:
			GLuint id;
			
		public:
			Framebuffer();
			virtual ~Framebuffer();
			
			unsigned int getId() const;
			void bind() const;
			
			void attachRenderbuffer( Renderbuffer * renderbuffer );
			void detachRenderbuffer();
			
			bool isComplete() const;
			void readPixels( int x, int y, int width, int height, vector<unsigned char> * pixels );
			
			static void unbind();
	};
}

#endif

