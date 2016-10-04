#ifndef __OPENGL_UI_BITMAPFONT_H
#define __OPENGL_UI_BITMAPFONT_H	1

#include <opengl/ui/Font.h>
#include <opengl/Program.h>
#include <opengl/TexturedRectangle.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

using namespace std;

namespace opengl
{
	namespace ui
	{
		class BitmapFont : public Font
		{
			protected:
				static unsigned int instances;
				static Program * program;
		
				ArrayBufferObject * vertices;
				ArrayBufferObject * textureCoordinates;
				ArrayBufferObject * colors;
				ElementArrayBufferObject * indices;
			
				vector<Point3D> vVertices;
				vector<Point2D> vTextureCoordinates;
				vector<Color> vColors;
				vector<unsigned short int> vIndices;
			
				TexturedRectangle * rectangle;
				unsigned int charactersByLine;
				unsigned int characterWidth;
				unsigned int characterHeight;
				unsigned int marginWidth;
				unsigned int marginHeight;
			
			public:
				BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight, unsigned int marginWidth = 0, unsigned int marginHeight = 0 );
				virtual ~BitmapFont();
		
				virtual void write( const Point3D& origin, const string& text, const Color& color, float size = 0.0f );
				virtual void getTextSize( Point2D& origin, const string& text, float size = 0.0f ) const;
				virtual float getTextWidth( const string& text, float size = 0.0f ) const;
				virtual float getTextHeight( const string& text, float size = 0.0f ) const;
				virtual void render();
			
				static string getFontNameFromPath( const string& filename );
		};
	}
}

#endif
