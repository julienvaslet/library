#ifndef __OPENGL_UI_LABEL_H
#define __OPENGL_UI_LABEL_H	1

#include <opengl/ui/Element.h>

#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

#include <vector>
using namespace std;

#define OPENGL_UI_LABEL_HORIZONTAL_PADDING	6
#define OPENGL_UI_LABEL_VERTICAL_PADDING	3

namespace opengl
{
	namespace ui
	{
		class Label : public Element
		{
			private:
				// this boolean should be thinked again
				// how to also manage global renderFunctions cache?
				//static bool useRenderingCache;
				
			protected:
				string value;
				Color color;
				
				virtual void autoResize();
			
			public:
				Label( const string& name, const string& value );
				virtual ~Label();
				
				const string& getValue() const;
				void setValue( const string& value, bool resize = false );
				
				void setColor( const string& color );
				void setColor( const Color& color );
				const Color& getColor() const;
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
		};
	}
}

#endif

