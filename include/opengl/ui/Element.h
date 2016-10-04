#ifndef __OPENGL_UI_ELEMENT_H
#define __OPENGL_UI_ELEMENT_H	1

#include <opengl/Rectangle.h>

namespace opengl
{
	namespace ui
	{
		class Element;
	}
}

#include <opengl/ui/UserInterface.h>
#include <opengl/ui/event/Event.h>

#include <string>
#include <map>
#include <set>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Element;
		typedef bool (*EventHandler)( Element *, const event::Event * );
		typedef void (*ElementRenderFunction)( unsigned int );
		
		class Element
		{
			protected:
				static set<ElementRenderFunction> renderFunctions;
				
				UserInterface * ui;
				string name;
				
				Rectangle rectangle;
				bool disabledState;
				bool autoSized;
				map<string, vector<EventHandler> > events;
				
				virtual void autoResize() = 0;
			
			public:
				Element( const string& name );
				virtual ~Element();
				
				const string& getName() const;
				virtual void setUserInterface( UserInterface * ui );
				UserInterface * getUserInterface();
				
				bool isAutoSized() const;
				void setAutoSized( bool state );
				
				const Rectangle& getRectangle() const;
				void resize( unsigned int width, unsigned int height );
				unsigned int getWidth() const;
				unsigned int getHeight() const;
				
				void moveTo( float x, float y, float z = 0.0f );
				float getX() const;
				float getY() const;
				float getZ() const;
				
				void addEventHandler( const string& event, EventHandler callback );
				void removeEventHandler( const string& event );
				void trigger( const event::Event * event );
				void trigger( const string& action );
				
				bool isDisabled() const;
				void setDisabledState( bool state );
				
				virtual void prepareRendering( unsigned int ticks ) = 0;
				static void render( unsigned int ticks );
		};
	}
}

#endif

