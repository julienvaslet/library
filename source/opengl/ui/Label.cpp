#include <opengl/ui/Label.h>

using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		//bool Label::useRenderingCache = true;
		
		Label::Label( const string& name, const string& value ) : Element(name), value(value), color(1.0f,1.0f,1.0f)
		{
			#ifdef DEBUG0
			Logger::get() << "[Label#" << this->name <<"] Created." << Logger::endl;
			#endif
		}
		
		Label::~Label()
		{
			#ifdef DEBUG0
			Logger::get() << "[Label#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		const string& Label::getValue() const
		{
			return this->value;
		}
		
		void Label::setValue( const string& value, bool resize )
		{
			this->value = value;
			
			if( resize )
				this->autoResize();
		}
		
		void Label::setColor( const string& color )
		{
			this->color.setColor( color );
		}
		
		void Label::setColor( const Color& color )
		{
			this->color.setColor( color );
		}
		
		const Color& Label::getColor() const
		{
			return this->color;
		}
		
		void Label::autoResize()
		{
			if( this->autoSized || this->getWidth() == 0 || this->getHeight() == 0 )
			{
				Point2D size;
			
				if( this->ui != NULL )
				{
					Font * font = this->ui->getFont();
				
					if( font != NULL )
						font->getTextSize( size, this->value, this->ui->getFontSize() );
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[Label#" << this->name << "] /!\\ The label is not linked to an UserInterface, so no font could be used." << Logger::endl;
				#endif
			
				this->rectangle.resize( size.getX() + OPENGL_UI_LABEL_HORIZONTAL_PADDING * 2, size.getY() + OPENGL_UI_LABEL_VERTICAL_PADDING * 2 );
			}
		}
		
		void Label::prepareRendering( unsigned int ticks )
		{
			Point3D origin( this->rectangle.getX() + OPENGL_UI_LABEL_HORIZONTAL_PADDING, this->rectangle.getY() + OPENGL_UI_LABEL_VERTICAL_PADDING, this->rectangle.getZ() + 0.1f );
			this->ui->getFont()->write( origin, this->value, this->color, this->ui->getFontSize() );
			
			// Nothing to render, all is delegated to Font.
			// Element::renderFunctions.insert( &Label::render );
		}
		
		void Label::render( unsigned int ticks )
		{
			// Nothing to do...
		}
	}
}
