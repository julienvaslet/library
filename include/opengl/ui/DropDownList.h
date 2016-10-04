#ifndef __OPENGL_UI_DROPDOWNLIST_H
#define __OPENGL_UI_DROPDOWNLIST_H	1

#include <opengl/ui/PushButton.h>
#include <opengl/ui/List.h>

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
		class DropDownList : public PushButton
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
				
				static void initializeRendering();
				
				static string getListName( const string& buttonName );
				static string getButtonName( const string& listName );
				
			protected:
				string title;
				List * list;
				
				virtual void autoResize();
				void synchronizeList();
				
			public:
				DropDownList( const string& name, const string& title );
				virtual ~DropDownList();
				
				virtual void setUserInterface( UserInterface * ui );
				
				void addItem( const string& value, bool selected = false );
				void setSelectedItem( int index );
				string getSelectedItem();
				void clear();
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
				
				static bool eventPushStateChanged( Element * element, const event::Event * event );
				static bool eventListSelectionChanged( Element * element, const event::Event * event );
		};
	}
}

#endif

