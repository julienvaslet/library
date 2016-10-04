#include <opengl/ui/event/Event.h>
#include <opengl/Screen.h>
#include <SDL2/SDL.h>

namespace opengl
{
	namespace ui
	{
		namespace event
		{
			Event::Event( const string& name ) : name(name)
			{
			}
		
			Event::~Event()
			{
			}
	
			const string& Event::getName() const
			{
				return this->name;
			}
			
			EventType Event::getType()
			{
				return EventType::Action;
			}
			
			Event * Event::create( const SDL_Event * event )
			{
				Event * e = NULL;
				
				switch( event->type )
				{
					case SDL_MOUSEMOTION:
					case SDL_MOUSEBUTTONUP:
					case SDL_MOUSEBUTTONDOWN:
					{
						e = MouseEvent::create( event );
						break;
					}
				}
			
				return e;
			}
		}
	}
}

