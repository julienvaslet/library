#ifndef __OPENGL_COLORED_RECTANGLE_H
#define __OPENGL_COLORED_RECTANGLE_H	1

#include <opengl/Rectangle.h>
#include <opengl/Color.h>

#include <opengl/Program.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

#include <vector>
using namespace std;

namespace opengl
{
	class ColoredRectangle : public Rectangle
	{
		protected:
			static unsigned int instances;
			static Program * program;
			static ArrayBufferObject * vertices;
			static ArrayBufferObject * colors;
			static ElementArrayBufferObject * indices;
			
			Color color;
			
			void initializeRendering();
		
		public:
			ColoredRectangle();
			ColoredRectangle( const ColoredRectangle& rectangle );
			ColoredRectangle( const Rectangle& rectangle );
			ColoredRectangle( const Rectangle& rectangle, const Color& color );
			ColoredRectangle( unsigned width, unsigned height );
			ColoredRectangle( unsigned width, unsigned height, const Color& color );
			virtual ~ColoredRectangle();
			
			Color& getColor();
			void setColor( const Color& color );
			
			void prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const;
			static void render( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices );
	};
}

#endif

