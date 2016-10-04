#ifndef __OPENGL_TEXTURE_H
#define __OPENGL_TEXTURE_H	1

#include <opengl/OpenGL.h>
#include <game/Resource.h>

using namespace std;

namespace opengl
{
	class Texture : public game::Resource
	{	
		protected:
			unsigned int id;
			
		public:
			Texture();
			virtual ~Texture();
		
			unsigned int getId() const;
			virtual void bind( unsigned int unit ) const = 0;
			virtual void setWrapMode( GLenum mode ) = 0;
			virtual void setFiltering( GLenum minification, GLenum magnification ) = 0;
	};
}

#endif
