#ifndef __OPENGL_BUFFEROBJECT_H
#define __OPENGL_BUFFEROBJECT_H	1

#include <opengl/OpenGL.h>

using namespace std;

namespace opengl
{
	class BufferObject
	{
		public:
			enum Frequency
			{
				Stream = 0,
				Static = 3,
				Dynamic = 6
			};
			
			enum Access
			{
#ifndef __PI__
				Read = 1,
				Copy = 2,
#endif
				Draw = 0
			};
		
		protected:
			GLuint buffer;
			
		public:
			BufferObject();
			virtual ~BufferObject();
			
			virtual void bind() = 0;
			GLuint getBuffer();
			static GLenum getUsage( Frequency f, Access a );
	};
}

#endif
