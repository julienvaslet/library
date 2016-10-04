#include <opengl/ui/Button.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define OPENGL_UI_BUTTON_VERTEX_SHADER				"data/shaders/ui/Button.vs"
#define OPENGL_UI_BUTTON_FRAGMENT_SHADER			"data/shaders/ui/Button.fs"
#define OPENGL_UI_BUTTON_HORIZONTAL_PADDING			8
#define OPENGL_UI_BUTTON_VERTICAL_PADDING			3
#define OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT		0.50
#define OPENGL_UI_BUTTON_HIGHLIGHT_COLOR_PERCENT	0.10

namespace opengl
{
	namespace ui
	{
		unsigned int Button::instances = 0;
		Program * Button::program = NULL;
		ArrayBufferObject * Button::vertices = NULL;
		ArrayBufferObject * Button::colors = NULL;
		ElementArrayBufferObject * Button::indices = NULL;
		
		vector<unsigned short int> Button::renderingIndices;
		vector<Point3D> Button::renderingVertices;
		vector<Color> Button::renderingColors;
		
		Button::Button( const string& name, const string& value ) : Label(name,value), backgroundColor("aaaaaa"), highlighted(false), pushed(false)
		{
			this->initializeRendering();
			
			this->addEventHandler( "mousedown", Button::eventMouseDown );
			this->addEventHandler( "mouseup", Button::eventMouseUp );
			this->addEventHandler( "mouseenter", Button::eventMouseEnter );
			this->addEventHandler( "mouseleave", Button::eventMouseLeave );
			
			#ifdef DEBUG0
			Logger::get() << "[Button#" << this->name << "] Created." << Logger::endl;
			#endif
		}
		
		Button::~Button()
		{
			Button::instances--;
			
			if( Button::instances == 0 )
			{
				if( Button::indices != NULL )
				{
					delete Button::indices;
					Button::indices = NULL;
				}
				
				if( Button::colors != NULL )
				{
					delete Button::colors;
					Button::colors = NULL;
				}
				
				if( Button::vertices != NULL )
				{
					delete Button::vertices;
					Button::vertices = NULL;
				}
				
				if( Button::program != NULL )
				{
					delete Button::program;
					Button::program = NULL;
				}
			}
			
			#ifdef DEBUG0
			Logger::get() << "[Button#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		void Button::initializeRendering()
		{
			Button::instances++;
			
			if( Button::program == NULL )
			{
				Button::program = new Program();
			
				Button::program->loadVertexShaderFile( OPENGL_UI_BUTTON_VERTEX_SHADER );
				Button::program->loadFragmentShaderFile( OPENGL_UI_BUTTON_FRAGMENT_SHADER );
		
				Button::program->link( true );
			}
	
			if( Button::vertices == NULL )
				Button::vertices = new ArrayBufferObject();
		
			if( Button::colors == NULL )
				Button::colors = new ArrayBufferObject();
		
			if( Button::indices == NULL )
				Button::indices = new ElementArrayBufferObject();
		}
		
		void Button::setBackgroundColor( const string& color )
		{
			this->backgroundColor.setColor( color );
		}
		
		void Button::setBackgroundColor( const Color& color )
		{
			this->backgroundColor.setColor( color );
		}
		
		const Color& Button::getBackgroundColor() const
		{
			return this->backgroundColor;
		}
		
		void Button::autoResize()
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
					Logger::get() << "[Button#" << this->name << "] /!\\ The button is not linked to an UserInterface, so no font could be used." << Logger::endl;
				#endif
			
				this->rectangle.resize( size.getX() + OPENGL_UI_BUTTON_HORIZONTAL_PADDING * 2, size.getY() + OPENGL_UI_BUTTON_VERTICAL_PADDING * 2 );
			}
		}
		
		// Rendering functions
		void Button::prepareRendering( unsigned int ticks )
		{
			//Logger::get() << "[Button#" << this->name << "] Preparing rendering..." << Logger::endl;
		
			// Temp, should only be done when a boolean is set (mustResize for instance)
			this->autoResize();
			
			// Button background
			unsigned int j = Button::renderingVertices.size();
			unsigned int width = this->rectangle.getWidth();
			unsigned int height = this->rectangle.getHeight();
			unsigned int border = 1;
			
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX(), this->rectangle.getY(), this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + width, this->rectangle.getY(), this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + width, this->rectangle.getY(), this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + width, this->rectangle.getY() + height, this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX(), this->rectangle.getY() + height, this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX(), this->rectangle.getY() + height, this->rectangle.getZ() ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, this->rectangle.getY() + border, this->rectangle.getZ() + 0.01f ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, this->rectangle.getY() + border, this->rectangle.getZ() + 0.01f ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, this->rectangle.getY() + height - border, this->rectangle.getZ() + 0.01f ) );
			Button::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, this->rectangle.getY() + height - border, this->rectangle.getZ() + 0.01f ) );
			
			Color topBorder( this->backgroundColor.getRed() + ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT), this->backgroundColor.getGreen() + ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT), this->backgroundColor.getBlue() + ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT) );
			Color bottomBorder( this->backgroundColor.getRed() - ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT), this->backgroundColor.getGreen() - ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT), this->backgroundColor.getBlue() - ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_BUTTON_BORDER_COLOR_PERCENT) );

			if( this->pushed )
			{
				Button::renderingColors.push_back( bottomBorder );
				Button::renderingColors.push_back( bottomBorder );
				Button::renderingColors.push_back( topBorder );
				Button::renderingColors.push_back( topBorder );
				Button::renderingColors.push_back( topBorder );
				Button::renderingColors.push_back( bottomBorder );
			}
			else
			{
				Button::renderingColors.push_back( topBorder );
				Button::renderingColors.push_back( topBorder );
				Button::renderingColors.push_back( bottomBorder );
				Button::renderingColors.push_back( bottomBorder );
				Button::renderingColors.push_back( bottomBorder );
				Button::renderingColors.push_back( topBorder );
			}
			
			if( this->highlighted )
			{
				Color highlight( this->backgroundColor.getRed() + ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_BUTTON_HIGHLIGHT_COLOR_PERCENT), this->backgroundColor.getGreen() + ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_BUTTON_HIGHLIGHT_COLOR_PERCENT), this->backgroundColor.getBlue() + ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_BUTTON_HIGHLIGHT_COLOR_PERCENT) ); 
				Button::renderingColors.push_back( highlight );
				Button::renderingColors.push_back( highlight );
				Button::renderingColors.push_back( highlight );
				Button::renderingColors.push_back( highlight );
			}
			else
			{
				Button::renderingColors.push_back( this->backgroundColor );
				Button::renderingColors.push_back( this->backgroundColor );
				Button::renderingColors.push_back( this->backgroundColor );
				Button::renderingColors.push_back( this->backgroundColor );
			}
			
			Button::renderingIndices.push_back( j );
			Button::renderingIndices.push_back( j + 1 );
			Button::renderingIndices.push_back( j + 5 );
			Button::renderingIndices.push_back( j + 2 );
			Button::renderingIndices.push_back( j + 3 );
			Button::renderingIndices.push_back( j + 4 );
			
			Button::renderingIndices.push_back( j + 6 );
			Button::renderingIndices.push_back( j + 7 );
			Button::renderingIndices.push_back( j + 8 );
			Button::renderingIndices.push_back( j + 6 );
			Button::renderingIndices.push_back( j + 8 );
			Button::renderingIndices.push_back( j + 9 );
			
			Point3D textOrigin( this->rectangle.getX() + OPENGL_UI_BUTTON_HORIZONTAL_PADDING + ( this->pushed ? 1 : 0 ), this->rectangle.getY() + OPENGL_UI_BUTTON_VERTICAL_PADDING + ( this->pushed ? 1 : 0 ), this->rectangle.getZ() + 0.1f );
			this->ui->getFont()->write( textOrigin, this->value, this->color, this->ui->getFontSize() );
			
			Element::renderFunctions.insert( &Button::render );
		}
		
		void Button::render( unsigned int ticks )
		{
			//Logger::get() << "[Button] Rendering" << Logger::endl;
			
			if( Button::program != NULL )
			{
				Button::program->use( true );
			
				Button::vertices->setData( Button::renderingVertices );
				Button::colors->setData( Button::renderingColors );
				Button::indices->setData( Button::renderingIndices );
			
				Button::program->sendUniform( "projection_matrix", UserInterface::get()->getProjectionMatrix(), false );
				Button::program->sendUniform( "modelview_matrix", UserInterface::get()->getModelviewMatrix(), false );
				Button::program->sendVertexPointer( "a_Vertex", Button::vertices );
				Button::program->sendColorPointer( "a_Color", Button::colors );

				Button::indices->draw( OpenGL::Triangles );
				
				Button::renderingVertices.clear();
				Button::renderingColors.clear();
				Button::renderingIndices.clear();
			}
		}
	
		// Events
		bool Button::eventMouseDown( Element * element, const event::Event * event )
		{
			const event::MouseEvent * e = reinterpret_cast<const event::MouseEvent *>( event );
			
			if( e->getButton() == event::MouseButton::Left )
			{
				Button * button = reinterpret_cast<Button *>( element );
				button->pushed = true;
			}
			
			return true;
		}
		
		bool Button::eventMouseUp( Element * element, const event::Event * event )
		{
			const event::MouseEvent * e = reinterpret_cast<const event::MouseEvent *>( event );
			
			if( e->getButton() == event::MouseButton::Left )
			{
				Button * button = reinterpret_cast<Button *>( element );
				button->pushed = false;
			}
			
			return true;
		}
		
		bool Button::eventMouseEnter( Element * element, const event::Event * event )
		{
			Button * button = reinterpret_cast<Button *>( element );
			button->highlighted = true;
			return true;
		}
		
		bool Button::eventMouseLeave( Element * element, const event::Event * event )
		{
			Button * button = reinterpret_cast<Button *>( element );
			button->highlighted = false;
			button->pushed = false;
			return true;
		}
	}
}

