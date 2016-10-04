#ifndef __OPENGL_UI_FONT_H
#define __OPENGL_UI_FONT_H	1

#include <opengl/OpenGL.h>
#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

#include <map>

using namespace std;

namespace opengl
{
	namespace ui
	{
		class Font
		{
			protected:
				static map<string, Font *> fonts;

				Font( const string& name );
				virtual ~Font();
			
			public:
				virtual void write( const Point3D& origin, const string& text, const Color& color, float size = 0.0f ) = 0;
				virtual void getTextSize( Point2D& origin, const string& text, float size = 0.0f ) const = 0;
				virtual float getTextWidth( const string& text, float size = 0.0f ) const = 0;
				virtual float getTextHeight( const string& text, float size = 0.0f ) const = 0;
				
				void write( const Point2D& origin, const string& text, const Color& color, float size = 0.0f )
				{
					Point3D newOrigin( origin.getX(), origin.getY(), 0.0f );
					this->write( newOrigin, text, color, size );
				}
				
				void write( const Point2D& origin, const string& text, float size = 0.0f )
				{
					Color color( 1.0f, 1.0f, 1.0f, 1.0f );
					this->write( origin, text, color, size );
				}
			
				virtual void render() = 0;
			
				static Font * get( const string& name );
				static void destroy( const string& name );
				static void destroy();
		};
	}
}

#endif
