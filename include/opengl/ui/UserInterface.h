#ifndef __OPENGL_UI_USERINTERFACE_H
#define __OPENGL_UI_USERINTERFACE_H	1

#define OPENGL_UI_DEFAULT_FONT			"bitmap"

namespace opengl
{
	namespace ui
	{
		class UserInterface;
	}
}

#include <opengl/ui/Element.h>
#include <opengl/ui/Font.h>
#include <opengl/Matrix.h>

#include <SDL2/SDL.h>

#include <map>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class UserInterface
		{
			protected:
				static UserInterface * userInterface;
			
				map<string, Element *> elements;
				set<string> mouseoverElements;
				set<string> hiddenElements;
				string fontName;
				float fontSize;
				
				unsigned int width;
				unsigned int height;
				Matrix projection;
				Matrix modelview;
				
				UserInterface( unsigned int width, unsigned int height );
				
			public:
				static bool create( unsigned int width = 0, unsigned int height = 0 );
				static UserInterface * get();
				virtual ~UserInterface();
				
				void resize( unsigned int width, unsigned int height );
				const Matrix& getProjectionMatrix();
				const Matrix& getModelviewMatrix();
				
				void setFont( const string& font, float fontSize = 0.0f );
				Font * getFont();
				float getFontSize();
				
				void addElement( Element * element, bool hidden = false );
				bool hasElement( const string& name );
				Element * getElement( const string& name );
				void removeElement( const string& name, bool deleteElement = true );
				void clear();
				
				void showElement( const string& name );
				void hideElement( const string& name );
				
				virtual void render( unsigned int ticks );
				bool dispatchEvent( const SDL_Event * event );
		};
	}
}

#endif

