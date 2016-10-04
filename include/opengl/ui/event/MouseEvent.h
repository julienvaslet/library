#ifndef __OPENGL_UI_EVENT_MOUSEEVENT_H
#define __OPENGL_UI_EVENT_MOUSEEVENT_H	1

#include <opengl/ui/event/Event.h>
#include <opengl/Point2D.h>
#include <SDL2/SDL.h>

namespace opengl
{
	namespace ui
	{
		namespace event
		{
			enum MouseButton
			{
				None,
				Left,
				Right,
				Middle
			};
			
			class MouseEvent : public Event
			{
				protected:
					Point2D origin;
					MouseButton button;
				
				public:
					MouseEvent( const string& name, MouseEvent * event = NULL );
					virtual ~MouseEvent();
					
					const Point2D& getOrigin() const;
					MouseButton getButton() const;
					
					virtual EventType getType();
					
					static MouseEvent * create( const SDL_Event * event );
			};
		}
	}
}

#endif

