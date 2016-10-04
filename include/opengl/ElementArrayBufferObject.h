#ifndef __OPENGL_ELEMENTARRAYBUFFEROBJECT_H
#define __OPENGL_ELEMENTARRAYBUFFEROBJECT_H	1

#include <opengl/OpenGL.h>
#include <opengl/BufferObject.h>

#include <vector>

using namespace std;

namespace opengl
{
	class ElementArrayBufferObject : public BufferObject
	{
		protected:
			unsigned int size;
			
		public:
			ElementArrayBufferObject();
			virtual ~ElementArrayBufferObject();
		
			virtual void bind();
			void setData( const vector<unsigned short int>& data, Frequency frequency = Static, Access access = Draw );

			void draw( unsigned int count = 0 );
			void draw( OpenGL::DrawMode mode, unsigned int count = 0 );
	};
}

#endif
