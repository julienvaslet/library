#ifndef __OPENGL_UI_LIST_H
#define __OPENGL_UI_LIST_H	1

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
		class List : public Label
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
				int selectedItem;
				int highlightedItem;
				vector<string> items;
			
				virtual void autoResize();
				
			public:
				List( const string& name );
				virtual ~List();
				
				void setBackgroundColor( const string& color );
				void setBackgroundColor( const Color& color );
				const Color& getBackgroundColor() const;
				
				void addItem( const string& value, bool selected = false );
				void setSelectedItem( int index );
				string getSelectedItem();
				void clear();
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
				
				static bool eventMousemove( Element * element, const event::Event * event );
				static bool eventMouseleave( Element * element, const event::Event * event );
				static bool eventMouseup( Element * element, const event::Event * event );
		};
	}
}

#endif

