#ifndef __OPENGL_TEXTURE2D_H
#define __OPENGL_TEXTURE2D_H	1

#include <opengl/Texture.h>
#include <data/image/Image.h>
#include <vector>

using namespace data::image;
using namespace std;

namespace opengl
{
	class Texture2D : public Texture
	{	
		protected:
			unsigned int width;
			unsigned int height;
			
		public:
			Texture2D();
			virtual ~Texture2D();
			
			unsigned int getWidth();
			unsigned int getHeight();
			
			virtual void bind( unsigned int unit = 0 ) const;
			virtual void setData( const Image& image );
			virtual void setData( const unsigned char * data, unsigned int width, unsigned int height, GLenum format = GL_RGB );
			virtual void setWrapMode( GLenum mode );
			virtual void setFiltering( GLenum minification, GLenum magnification );
	};
}

#endif
