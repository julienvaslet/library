#ifndef __OPENGL_UI_PUSHBUTTON_H
#define __OPENGL_UI_PUSHBUTTON_H	1

#include <opengl/ui/Button.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class PushButton : public Button
		{				
			protected:
				bool pushState;

			public:
				PushButton( const string& name, const string& value, bool pushed = false );
				virtual ~PushButton();
				
				void setPushState( bool state );
				bool getPushState() const;
			
				static bool eventMouseUp( Element * element, const event::Event * event );
				static bool eventMouseLeave( Element * element, const event::Event * event );
		};
	}
}

#endif

