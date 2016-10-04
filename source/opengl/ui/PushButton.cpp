#include <opengl/ui/PushButton.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		PushButton::PushButton( const string& name, const string& value, bool pushed ) : Button(name,value), pushState(pushed)
		{
			this->pushed = this->pushState;
			
			this->removeEventHandler( "mouseup" );
			this->addEventHandler( "mouseup", PushButton::eventMouseUp );
		
			this->removeEventHandler( "mouseleave" );
			this->addEventHandler( "mouseleave", PushButton::eventMouseLeave );
			
			#ifdef DEBUG0
			cout << "[PushButton#" << this->name << "] Created." << endl;
			#endif
		}
	
		PushButton::~PushButton()
		{
			#ifdef DEBUG0
			cout << "[PushButton#" << this->name << "] Deleted." << endl;
			#endif
		}
	
		void PushButton::setPushState( bool state )
		{
			this->pushState = state;
			this->pushed = this->pushState;
			
			this->trigger( "pushstatechanged" );
		}
	
		bool PushButton::getPushState() const
		{
			return this->pushState;
		}
	
		bool PushButton::eventMouseUp( Element * element, const event::Event * event )
		{
			const event::MouseEvent * e = reinterpret_cast<const event::MouseEvent *>( event );
			
			if( e->getButton() == event::MouseButton::Left )
			{
				PushButton * button = reinterpret_cast<PushButton *>( element );
				button->pushState = !button->pushState;
				button->pushed = button->pushState;
				
				button->trigger( "pushstatechanged" );
			}
			
			return true;
		}
	
		bool PushButton::eventMouseLeave( Element * element, const event::Event * event )
		{
			PushButton * button = reinterpret_cast<PushButton *>( element );
			button->highlighted = false;
			button->pushed = button->pushState;
			return true;
		}
	}
}

