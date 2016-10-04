#ifndef __OPENGL_UI_BUTTON_H
#define __OPENGL_UI_BUTTON_H	1

#include <opengl/ui/Label.h>

#include <opengl/Program.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Button : public Label
		{
			private:
				static unsigned int instances;
				static Program * program;
				static ArrayBufferObject * vertices;
				static ArrayBufferObject * colors;
				static ElementArrayBufferObject * indices;
				
				static vector<unsigned short int> renderingIndices;
				static vector<Point3D> renderingVertices;
				static vector<Color> renderingColors;
				
				void initializeRendering();
				
			protected:
				Color backgroundColor;
				bool highlighted;
				bool pushed;
				
				virtual void autoResize();
			
			public:
				Button( const string& name, const string& value );
				virtual ~Button();
				
				void setBackgroundColor( const string& color );
				void setBackgroundColor( const Color& color );
				const Color& getBackgroundColor() const;
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
			
				static bool eventMouseDown( Element * element, const event::Event * event );
				static bool eventMouseUp( Element * element, const event::Event * event );
				static bool eventMouseEnter( Element * element, const event::Event * event );
				static bool eventMouseLeave( Element * element, const event::Event * event );
		};
	}
}

#endif
