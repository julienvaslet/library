#ifndef __OPENGL_ARRAYBUFFEROBJECT_H
#define __OPENGL_ARRAYBUFFEROBJECT_H	1

#ifdef __PI__
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include <vector>

#include <opengl/BufferObject.h>
#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

using namespace std;

namespace opengl
{
	class ArrayBufferObject : public BufferObject
	{			
		public:
			ArrayBufferObject();
			virtual ~ArrayBufferObject();
			
			virtual void bind();
			void setData( const vector<float>& data, Frequency frequency = Static, Access access = Draw );
			void setData( const vector<Point2D>& data, Frequency frequency = Static, Access access = Draw );
			void setData( const vector<Point3D>& data, Frequency frequency = Static, Access access = Draw );
			void setData( const vector<Color>& data, Frequency frequency = Static, Access access = Draw );
	};
}

#endif
