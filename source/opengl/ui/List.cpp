#include <opengl/ui/List.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define OPENGL_UI_LIST_VERTEX_SHADER				"data/shaders/ui/Button.vs"
#define OPENGL_UI_LIST_FRAGMENT_SHADER				"data/shaders/ui/Button.fs"
#define OPENGL_UI_LIST_ELEMENT_HORIZONTAL_PADDING	8.0f
#define OPENGL_UI_LIST_ELEMENT_VERTICAL_PADDING		3.0f
#define OPENGL_UI_LIST_HORIZONTAL_PADDING			0.0f
#define OPENGL_UI_LIST_VERTICAL_PADDING				2.0f
#define OPENGL_UI_LIST_BORDER_COLOR_PERCENT			0.50f
#define OPENGL_UI_LIST_HIGHLIGHT_COLOR_PERCENT		0.10f

namespace opengl
{
	namespace ui
	{
		unsigned int List::instances = 0;
		Program * List::program = NULL;
		ArrayBufferObject * List::vertices = NULL;
		ArrayBufferObject * List::colors = NULL;
		ElementArrayBufferObject * List::indices = NULL;
		
		vector<unsigned short int> List::renderingIndices;
		vector<Point3D> List::renderingVertices;
		vector<Color> List::renderingColors;
		
		List::List( const string& name ) : Label(name, ""), backgroundColor("aaaaaa"), selectedItem(-1), highlightedItem(-1)
		{
			this->initializeRendering();
			
			this->addEventHandler( "mousemove", List::eventMousemove );
			this->addEventHandler( "mouseleave", List::eventMouseleave );
			this->addEventHandler( "mouseup", List::eventMouseup );
			
			#ifdef DEBUG0
			Logger::get() << "[List#" << this->name << "] Created." << Logger::endl;
			#endif
		}
		
		List::~List()
		{
			List::instances--;
			
			if( List::instances == 0 )
			{
				if( List::indices != NULL )
				{
					delete List::indices;
					List::indices = NULL;
				}
				
				if( List::colors != NULL )
				{
					delete List::colors;
					List::colors = NULL;
				}
				
				if( List::vertices != NULL )
				{
					delete List::vertices;
					List::vertices = NULL;
				}
				
				if( List::program != NULL )
				{
					delete List::program;
					List::program = NULL;
				}
			}
			
			#ifdef DEBUG0
			Logger::get() << "[List#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		void List::initializeRendering()
		{
			List::instances++;
			
			if( List::program == NULL )
			{
				List::program = new Program();
			
				List::program->loadVertexShaderFile( OPENGL_UI_LIST_VERTEX_SHADER );
				List::program->loadFragmentShaderFile( OPENGL_UI_LIST_FRAGMENT_SHADER );
		
				List::program->link( true );
			}
	
			if( List::vertices == NULL )
				List::vertices = new ArrayBufferObject();
		
			if( List::colors == NULL )
				List::colors = new ArrayBufferObject();
		
			if( List::indices == NULL )
				List::indices = new ElementArrayBufferObject();
		}
		
		void List::setBackgroundColor( const string& color )
		{
			this->backgroundColor.setColor( color );
		}
		
		void List::setBackgroundColor( const Color& color )
		{
			this->backgroundColor.setColor( color );
		}
		
		const Color& List::getBackgroundColor() const
		{
			return this->backgroundColor;
		}
		
		void List::autoResize()
		{
			Point2D size;
			
			if( this->ui != NULL )
			{
				Font * font = this->ui->getFont();
				
				if( font != NULL )
				{
					string longestValue = this->value;
					
					// We consider using monospaced font (performance)
					for( vector<string>::const_iterator it = this->items.begin() ; it != this->items.end() ; it++ )
					{
						if( (*it).length() > longestValue.length() )
							longestValue = (*it);
					}
					
					font->getTextSize( size, longestValue, this->ui->getFontSize() );
				}
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[List#" << this->name << "] /!\\ The list is not linked to an UserInterface, so no font could be used." << Logger::endl;
			#endif
			
			this->rectangle.resize( size.getX() + OPENGL_UI_LIST_ELEMENT_HORIZONTAL_PADDING * 2 + OPENGL_UI_LIST_HORIZONTAL_PADDING * 2, (size.getY() + OPENGL_UI_LIST_ELEMENT_VERTICAL_PADDING * 2) * this->items.size() + OPENGL_UI_LIST_VERTICAL_PADDING * 2 );
		}
		
		void List::addItem( const string& item, bool selected )
		{
			this->items.push_back( item );
			
			if( selected )
				this->selectedItem = this->items.size() - 1;
		}
		
		void List::setSelectedItem( int index )
		{
			bool triggerEvent = (index != this->selectedItem);
			
			if( index < 0 || static_cast<unsigned int>( index ) >= this->items.size() )
				this->selectedItem = -1;
			else
				this->selectedItem = index;
				
			if( triggerEvent )
				this->trigger( "selectionchanged" );
		}
		
		string List::getSelectedItem()
		{
			if( this->selectedItem >= 0 && static_cast<unsigned int>( this->selectedItem ) < this->items.size() )
			{
				return this->items[this->selectedItem];
			}
			else
				return "";
		}
		
		void List::clear()
		{
			this->selectedItem = -1;
			this->items.clear();
		}
		
		void List::prepareRendering( unsigned int ticks )
		{
			this->autoResize();
			
			unsigned int j = List::renderingVertices.size();
			unsigned int width = this->rectangle.getWidth();
			unsigned int height = this->rectangle.getHeight();
			unsigned int border = 1;
			
			List::renderingVertices.push_back( Point3D( this->rectangle.getX(), this->rectangle.getY(), this->rectangle.getZ() ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width, this->rectangle.getY(), this->rectangle.getZ() ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width, this->rectangle.getY() + height, this->rectangle.getZ() ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX(), this->rectangle.getY() + height, this->rectangle.getZ() ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, this->rectangle.getY() + border, this->rectangle.getZ() + 0.01f ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, this->rectangle.getY() + border, this->rectangle.getZ() + 0.01f ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, this->rectangle.getY() + height - border, this->rectangle.getZ() + 0.01f ) );
			List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, this->rectangle.getY() + height - border, this->rectangle.getZ() + 0.01f ) );
			
			Color borderColor( this->backgroundColor.getRed() - ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT), this->backgroundColor.getGreen() - ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT), this->backgroundColor.getBlue() - ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT) );

			List::renderingColors.push_back( borderColor );
			List::renderingColors.push_back( borderColor );
			List::renderingColors.push_back( borderColor );
			List::renderingColors.push_back( borderColor );
			List::renderingColors.push_back( this->backgroundColor );
			List::renderingColors.push_back( this->backgroundColor );
			List::renderingColors.push_back( this->backgroundColor );
			List::renderingColors.push_back( this->backgroundColor );
			
			List::renderingIndices.push_back( j );
			List::renderingIndices.push_back( j + 1 );
			List::renderingIndices.push_back( j + 2 );
			List::renderingIndices.push_back( j );
			List::renderingIndices.push_back( j + 2 );
			List::renderingIndices.push_back( j + 3 );
			
			List::renderingIndices.push_back( j + 4 );
			List::renderingIndices.push_back( j + 5 );
			List::renderingIndices.push_back( j + 6 );
			List::renderingIndices.push_back( j + 4 );
			List::renderingIndices.push_back( j + 7 );
			List::renderingIndices.push_back( j + 6 );
			
			if( this->highlightedItem >= 0 )
			{
				Color highlightColor( this->backgroundColor.getRed() + ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_LIST_HIGHLIGHT_COLOR_PERCENT), this->backgroundColor.getGreen() + ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_LIST_HIGHLIGHT_COLOR_PERCENT), this->backgroundColor.getBlue() + ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_LIST_HIGHLIGHT_COLOR_PERCENT) );
				float cellHeight = (this->rectangle.getHeight() - OPENGL_UI_LIST_VERTICAL_PADDING * 2.0f) / static_cast<float>( this->items.size() );
				float yDelta = cellHeight * this->highlightedItem;
				
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.02f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.02f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + cellHeight + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.02f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + cellHeight + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.02f ) );
				
				List::renderingColors.push_back( highlightColor );
				List::renderingColors.push_back( highlightColor );
				List::renderingColors.push_back( highlightColor );
				List::renderingColors.push_back( highlightColor );
				
				List::renderingIndices.push_back( j + 8 );
				List::renderingIndices.push_back( j + 9 );
				List::renderingIndices.push_back( j + 10 );
				List::renderingIndices.push_back( j + 8 );
				List::renderingIndices.push_back( j + 11 );
				List::renderingIndices.push_back( j + 10 );
			}
			
			if( this->selectedItem >= 0 )
			{
				Color selectedBorder( this->backgroundColor.getRed() + ((1.0f - this->backgroundColor.getRed()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT), this->backgroundColor.getGreen() + ((1.0f - this->backgroundColor.getGreen()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT), this->backgroundColor.getBlue() + ((1.0f - this->backgroundColor.getBlue()) * OPENGL_UI_LIST_BORDER_COLOR_PERCENT) );
				float cellHeight = (this->rectangle.getHeight() - OPENGL_UI_LIST_VERTICAL_PADDING * 2.0f) / static_cast<float>( this->items.size() );
				float yDelta = cellHeight * this->selectedItem;
				unsigned short int indiceDelta = ( this->highlightedItem >= 0 ) ? 4 : 0;
				
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING + border, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING + border, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + cellHeight - border + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + cellHeight - border + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - border, yDelta + cellHeight + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				List::renderingVertices.push_back( Point3D( this->rectangle.getX() + border, yDelta + cellHeight + this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING, this->rectangle.getZ() + 0.03f ) );
				
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				List::renderingColors.push_back( selectedBorder );
				
				List::renderingIndices.push_back( j + indiceDelta + 8 );
				List::renderingIndices.push_back( j + indiceDelta + 9 );
				List::renderingIndices.push_back( j + indiceDelta + 10 );
				List::renderingIndices.push_back( j + indiceDelta + 8 );
				List::renderingIndices.push_back( j + indiceDelta + 11 );
				List::renderingIndices.push_back( j + indiceDelta + 10 );
				List::renderingIndices.push_back( j + indiceDelta + 12 );
				List::renderingIndices.push_back( j + indiceDelta + 13 );
				List::renderingIndices.push_back( j + indiceDelta + 14 );
				List::renderingIndices.push_back( j + indiceDelta + 12 );
				List::renderingIndices.push_back( j + indiceDelta + 15 );
				List::renderingIndices.push_back( j + indiceDelta + 14 );
			}
				
			Point3D textOrigin( this->rectangle.getX() + OPENGL_UI_LIST_HORIZONTAL_PADDING + OPENGL_UI_LIST_ELEMENT_HORIZONTAL_PADDING, this->rectangle.getY() + OPENGL_UI_LIST_VERTICAL_PADDING + OPENGL_UI_LIST_ELEMENT_VERTICAL_PADDING, this->rectangle.getZ() + 0.1f );
			
			for( vector<string>::iterator it = this->items.begin() ; it != this->items.end() ; it++ )
			{
				float height = this->ui->getFont()->getTextHeight( *it, this->ui->getFontSize() );
				this->ui->getFont()->write( textOrigin, *it, this->color, this->ui->getFontSize() );
				
				textOrigin.moveBy( 0.0f, height + OPENGL_UI_LIST_ELEMENT_VERTICAL_PADDING * 2, 0.0f );
			}
			
			Element::renderFunctions.insert( &List::render );
		}
		
		void List::render( unsigned int ticks )
		{
			if( List::program != NULL )
			{
				List::program->use( true );
			
				List::vertices->setData( List::renderingVertices );
				List::colors->setData( List::renderingColors );
				List::indices->setData( List::renderingIndices );
			
			
				List::program->sendUniform( "projection_matrix", UserInterface::get()->getProjectionMatrix(), false );
				List::program->sendUniform( "modelview_matrix", UserInterface::get()->getModelviewMatrix(), false );
				List::program->sendVertexPointer( "a_Vertex", List::vertices );
				List::program->sendColorPointer( "a_Color", List::colors );

				List::indices->draw( OpenGL::Triangles );
				
				List::renderingVertices.clear();
				List::renderingColors.clear();
				List::renderingIndices.clear();
			}
		}
		
		bool List::eventMousemove( Element * element, const event::Event * event )
		{
			const event::MouseEvent * mEvent = reinterpret_cast<const event::MouseEvent *>( event );
			List * list = reinterpret_cast<List *>( element );
			
			int x = mEvent->getOrigin().getX() - list->rectangle.getOrigin().getX();
			int y = mEvent->getOrigin().getY() - list->rectangle.getOrigin().getY();
			
			if( x > OPENGL_UI_LIST_HORIZONTAL_PADDING
			 && x < list->rectangle.getWidth() - OPENGL_UI_LIST_HORIZONTAL_PADDING
			 && y > OPENGL_UI_LIST_VERTICAL_PADDING
			 && y < list->rectangle.getHeight() - OPENGL_UI_LIST_VERTICAL_PADDING )
			{
				float itemHeight = (list->rectangle.getHeight() - OPENGL_UI_LIST_VERTICAL_PADDING * 2.0f) / static_cast<float>( list->items.size() );
				list->highlightedItem = static_cast<int>( floor( (y - OPENGL_UI_LIST_VERTICAL_PADDING) / itemHeight ) );
			}
			else
				list->highlightedItem = -1;
			
			return true;
		}
		
		bool List::eventMouseleave( Element * element, const event::Event * event )
		{
			List * list = reinterpret_cast<List *>( element );
			list->highlightedItem = -1;
			
			return true;
		}
		
		bool List::eventMouseup( Element * element, const event::Event * event )
		{
			List * list = reinterpret_cast<List *>( element );
			
			if( list->highlightedItem >= 0 && static_cast<unsigned int>( list->highlightedItem ) < list->items.size() )
			{
				list->selectedItem = list->highlightedItem;
				
				list->trigger( "selectionchanged" );
			}
			
			return true;
		}
	}
}

