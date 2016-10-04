#include <opengl/ui/event/MouseEvent.h>
#include <opengl/Screen.h>

namespace opengl
{
	namespace ui
	{
		namespace event
		{
			MouseEvent::MouseEvent( const string& name, MouseEvent * event ) : Event(name)
			{
				if( event != NULL )
				{
					this->origin = event->origin;
					this->button = event->button;
				}
			}
			
			MouseEvent::~MouseEvent()
			{
			}
			
			const Point2D& MouseEvent::getOrigin() const
			{
				return this->origin;
			}
			
			MouseButton MouseEvent::getButton() const
			{
				return this->button;
			}
			
			EventType MouseEvent::getType()
			{
				return EventType::Mouse;
			}
					
			MouseEvent * MouseEvent::create( const SDL_Event * event )
			{
				MouseEvent * e = NULL;
				
				switch( event->type )
				{
					case SDL_MOUSEMOTION:
					{
						e = new MouseEvent( "mousemove" );
						e->origin = Screen::get()->getCoordinates( event->motion.x, event->motion.y );
						
						break;
					}
			
					case SDL_MOUSEBUTTONUP:
					case SDL_MOUSEBUTTONDOWN:
					{
						e = new MouseEvent( event->type == SDL_MOUSEBUTTONUP ? "mouseup" : "mousedown" );
						e->origin = Screen::get()->getCoordinates( event->button.x, event->button.y );
						
						switch( event->button.button )
						{
							case SDL_BUTTON_LEFT:
							{
								e->button = MouseButton::Left;
								break;
							}
							
							case SDL_BUTTON_RIGHT:
							{
								e->button = MouseButton::Right;
								break;
							}
							
							case SDL_BUTTON_MIDDLE:
							{
								e->button = MouseButton::Middle;
								break;
							}
							
							default:
							{
								e->button = MouseButton::None;
								break;
							}
						}
					
						break;
					}
				}
				
				return e;
			}
		}
	}
}

