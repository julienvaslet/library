#include <opengl/ui/Element.h>
#include <opengl/ui/UserInterface.h>

#include <vector>
using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		set<ElementRenderFunction> Element::renderFunctions;
		
		Element::Element( const string& name ) : ui(NULL), name(name), disabledState(false), autoSized(true)
		{
		}
		
		Element::~Element()
		{
		}
		
		const string& Element::getName() const
		{
			return this->name;
		}
		
		bool Element::isDisabled() const
		{
			return this->disabledState;
		}
		
		void Element::setDisabledState( bool state )
		{
			this->disabledState = state;
		}
		
		void Element::setUserInterface( UserInterface * ui )
		{
			this->ui = ui;
		}
		
		UserInterface * Element::getUserInterface()
		{
			return this->ui;
		}
		
		bool Element::isAutoSized() const
		{
			return this->autoSized;
		}
		
		void Element::setAutoSized( bool state )
		{
			this->autoSized = state;
		}
		
		const Rectangle& Element::getRectangle() const
		{
			return this->rectangle;
		}
		
		void Element::resize( unsigned int width, unsigned int height )
		{
			this->rectangle.resize( width, height );
			this->autoSized = false;
		}
		
		unsigned int Element::getWidth() const
		{
			return this->rectangle.getWidth();
		}
		
		unsigned int Element::getHeight() const
		{
			return this->rectangle.getHeight();
		}
		
		void Element::moveTo( float x, float y, float z )
		{
			this->rectangle.getOrigin().moveTo( x, y, z );
		}
		
		float Element::getX() const
		{
			return this->rectangle.getX();
		}
		
		float Element::getY() const
		{
			return this->rectangle.getY();
		}
		
		float Element::getZ() const
		{
			return this->rectangle.getZ();
		}
		
		void Element::render( unsigned int ticks )
		{
			for( set<ElementRenderFunction>::iterator it = Element::renderFunctions.begin() ; it != Element::renderFunctions.end() ; it++ )
				(*it)( ticks );
				
			Element::renderFunctions.clear();
		}
		
		void Element::addEventHandler( const string& event, EventHandler callback )
		{
			map<string, vector<EventHandler> >::iterator it = this->events.find( event );
		
			if( it == this->events.end() )
			{
				vector<EventHandler> evts;
				evts.push_back( callback );
				this->events[event] = evts;
			}
			else
				this->events[event].push_back( callback );
		}
		
		void Element::removeEventHandler( const string& event )
		{
			map<string, vector<EventHandler> >::iterator it = this->events.find( event );
		
			if( it != this->events.end() )
				this->events.erase( it );
		}
		
		void Element::trigger( const event::Event * event )
		{
			map<string, vector<EventHandler> >::iterator it = this->events.find( event->getName() );
		
			if( it != this->events.end() )
			{			
				for( vector<EventHandler>::reverse_iterator itEvent = it->second.rbegin() ; itEvent != it->second.rend() ; itEvent++ )
				{
					if( *itEvent != NULL )
					{
						// This could trigger a bad behavior if events are badly setted.
						if( (*itEvent)( this, event ) == false )
						{
							#ifdef DEBUG0
							cout << "[Element#" << this->getName() << "] Event propagation stopped." << endl;
							#endif
						
							break;
						}
					}
					else
						break;
				}
			}
		}
		
		void Element::trigger( const string& action )
		{
			event::Event * event = new event::Event( action );
			this->trigger( event );
			delete event;
		}
	}
}
