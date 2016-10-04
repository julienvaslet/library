#ifndef __OPENGL_UI_EVENT_EVENT_H
#define __OPENGL_UI_EVENT_EVENT_H	1

#include <SDL2/SDL.h>

#include <string>
using namespace std;

namespace opengl
{
	namespace ui
	{
		namespace event
		{
			enum EventType
			{
				Action,
				Mouse,
				Keyboard,
				Window
			};
		
			class Event
			{
				protected:
					string name;
			
				public:
					Event( const string& name );
					virtual ~Event();
				
					const string& getName() const;
					virtual EventType getType();
					
					static Event * create( const SDL_Event * event );
			};
		}
	}
}

#include <opengl/ui/event/MouseEvent.h>

#endif
