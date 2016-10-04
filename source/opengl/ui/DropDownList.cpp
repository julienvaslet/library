#include <opengl/ui/DropDownList.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define OPENGL_UI_DROPDOWNLIST_VERTEX_SHADER			"data/shaders/ui/Button.vs"
#define OPENGL_UI_DROPDOWNLIST_FRAGMENT_SHADER			"data/shaders/ui/Button.fs"
#define OPENGL_UI_DROPDOWNLIST_HORIZONTAL_PADDING		8.0f
#define OPENGL_UI_DROPDOWNLIST_VERTICAL_PADDING			3.0f
#define OPENGL_UI_DROPDOWNLIST_TRIANGLE_WIDTH			10.0f
#define OPENGL_UI_DROPDOWNLIST_TRIANGLE_HEIGHT			5.0f

namespace opengl
{
	namespace ui
	{
		unsigned int DropDownList::instances = 0;
		Program * DropDownList::program = NULL;
		ArrayBufferObject * DropDownList::vertices = NULL;
		ArrayBufferObject * DropDownList::colors = NULL;
		ElementArrayBufferObject * DropDownList::indices = NULL;
		
		vector<unsigned short int> DropDownList::renderingIndices;
		vector<Point3D> DropDownList::renderingVertices;
		vector<Color> DropDownList::renderingColors;
		
		DropDownList::DropDownList( const string& name, const string& title ) : PushButton(name, title), title(title), list(NULL)
		{
			DropDownList::initializeRendering();
			this->list = new List( DropDownList::getListName( this->name ) );
			
			this->list->addEventHandler( "selectionchanged", DropDownList::eventListSelectionChanged );
			this->addEventHandler( "pushstatechanged", DropDownList::eventPushStateChanged );
			
			#ifdef DEBUG0
			Logger::get() << "[DropDownList#" << this->name << "] Created." << Logger::endl;
			#endif
		}
		
		DropDownList::~DropDownList()
		{
			if( this->list != NULL && this->ui->hasElement( this->list->getName() ) )
				this->ui->removeElement( this->list->getName() );
			
			DropDownList::instances--;
			
			if( DropDownList::instances == 0 )
			{
				if( DropDownList::indices != NULL )
				{
					delete DropDownList::indices;
					DropDownList::indices = NULL;
				}
				
				if( DropDownList::colors != NULL )
				{
					delete DropDownList::colors;
					DropDownList::colors = NULL;
				}
				
				if( DropDownList::vertices != NULL )
				{
					delete DropDownList::vertices;
					DropDownList::vertices = NULL;
				}
				
				if( DropDownList::program != NULL )
				{
					delete DropDownList::program;
					DropDownList::program = NULL;
				}
			}
			
			#ifdef DEBUG0
			Logger::get() << "[DropDownList#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		void DropDownList::initializeRendering()
		{
			DropDownList::instances++;
			
			if( DropDownList::program == NULL )
			{
				DropDownList::program = new Program();
			
				DropDownList::program->loadVertexShaderFile( OPENGL_UI_DROPDOWNLIST_VERTEX_SHADER );
				DropDownList::program->loadFragmentShaderFile( OPENGL_UI_DROPDOWNLIST_FRAGMENT_SHADER );
		
				DropDownList::program->link( true );
			}
	
			if( DropDownList::vertices == NULL )
				DropDownList::vertices = new ArrayBufferObject();
		
			if( DropDownList::colors == NULL )
				DropDownList::colors = new ArrayBufferObject();
		
			if( DropDownList::indices == NULL )
				DropDownList::indices = new ElementArrayBufferObject();
		}
		
		string DropDownList::getListName( const string& buttonName )
		{
			return buttonName + "_list";
		}
		
		string DropDownList::getButtonName( const string& listName )
		{
			return listName.substr( 0, listName.length() - 5 );
		}
		
		void DropDownList::synchronizeList()
		{
			// Update value
			this->value = this->list->getSelectedItem();
			
			if( this->value.length() == 0 )
				this->value = this->title;
		
			// Update list positions & colors
			this->list->setBackgroundColor( this->backgroundColor );
			this->list->setColor( this->color );
			this->list->moveTo( this->rectangle.getOrigin().getX(), this->rectangle.getOrigin().getY() + this->rectangle.getHeight(), this->rectangle.getOrigin().getZ() + 1.0f );
		}
		
		void DropDownList::setUserInterface( UserInterface * ui )
		{
			Element::setUserInterface( ui );
			
			this->ui->addElement( this->list, !this->pushState );
		}
		
		void DropDownList::autoResize()
		{
			if( this->autoSized || this->getWidth() == 0 || this->getHeight() == 0 )
			{
				Point2D size;
			
				if( this->ui != NULL )
				{
					Font * font = this->ui->getFont();
				
					if( font != NULL )
					{
						string longestValue = this->value;
					
						if( this->value.length() == 0 )
							longestValue = this->title;
					
						font->getTextSize( size, longestValue, this->ui->getFontSize() );
					}
				}
				#ifdef DEBUG0
				else
					Logger::get() << "[DropDownList#" << this->name << "] /!\\ The drop-down list is not linked to an UserInterface, so no font could be used." << Logger::endl;
				#endif
			
				this->rectangle.resize( size.getX() + OPENGL_UI_DROPDOWNLIST_HORIZONTAL_PADDING * 3 + OPENGL_UI_DROPDOWNLIST_TRIANGLE_WIDTH, size.getY() + OPENGL_UI_DROPDOWNLIST_VERTICAL_PADDING * 2 );
			}
		}
		
		void DropDownList::addItem( const string& item, bool selected )
		{
			this->list->addItem( item, selected );
			this->synchronizeList();
		}
		
		void DropDownList::setSelectedItem( int index )
		{
			this->list->setSelectedItem( index );
			this->synchronizeList();
		}
		
		string DropDownList::getSelectedItem()
		{
			return this->list->getSelectedItem();
		}
		
		void DropDownList::clear()
		{
			this->list->clear();
		}
		
		void DropDownList::prepareRendering( unsigned int ticks )
		{
			PushButton::prepareRendering( ticks );
			
			// Temp, should only be done when a boolean is set (mustResize for instance)
			this->autoResize();
			
			unsigned int j = DropDownList::renderingVertices.size();
			unsigned int width = this->rectangle.getWidth();
			unsigned int height = this->rectangle.getHeight();
			
			DropDownList::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - OPENGL_UI_DROPDOWNLIST_HORIZONTAL_PADDING - OPENGL_UI_DROPDOWNLIST_TRIANGLE_WIDTH + (this->pushed ? 1 : 0), this->rectangle.getY() + ((height - OPENGL_UI_DROPDOWNLIST_VERTICAL_PADDING - OPENGL_UI_DROPDOWNLIST_TRIANGLE_HEIGHT) / 2.0f) + 1 + (this->pushed ? 1 : 0), this->rectangle.getZ() + 0.02f ) );
			DropDownList::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - OPENGL_UI_DROPDOWNLIST_HORIZONTAL_PADDING + (this->pushed ? 1 : 0), this->rectangle.getY() + ((height - OPENGL_UI_DROPDOWNLIST_VERTICAL_PADDING - OPENGL_UI_DROPDOWNLIST_TRIANGLE_HEIGHT) / 2.0f) + 1 + (this->pushed ? 1 : 0), this->rectangle.getZ() + 0.02f ) );
			DropDownList::renderingVertices.push_back( Point3D( this->rectangle.getX() + width - OPENGL_UI_DROPDOWNLIST_HORIZONTAL_PADDING - (OPENGL_UI_DROPDOWNLIST_TRIANGLE_WIDTH / 2.0f) + (this->pushed ? 1 : 0), this->rectangle.getY() + height - ( (height - OPENGL_UI_DROPDOWNLIST_VERTICAL_PADDING - OPENGL_UI_DROPDOWNLIST_TRIANGLE_HEIGHT) / 2.0f ) + 1 + (this->pushed ? 1 : 0), this->rectangle.getZ() + 0.02f ) );

			DropDownList::renderingColors.push_back( this->color );
			DropDownList::renderingColors.push_back( this->color );
			DropDownList::renderingColors.push_back( this->color );
			
			DropDownList::renderingIndices.push_back( j );
			DropDownList::renderingIndices.push_back( j + 1 );
			DropDownList::renderingIndices.push_back( j + 2 );
			
			Element::renderFunctions.insert( &DropDownList::render );
		}
		
		void DropDownList::render( unsigned int ticks )
		{
			if( DropDownList::program != NULL )
			{
				DropDownList::program->use( true );
			
				DropDownList::vertices->setData( DropDownList::renderingVertices );
				DropDownList::colors->setData( DropDownList::renderingColors );
				DropDownList::indices->setData( DropDownList::renderingIndices );
				
				DropDownList::program->sendUniform( "projection_matrix", UserInterface::get()->getProjectionMatrix(), false );
				DropDownList::program->sendUniform( "modelview_matrix", UserInterface::get()->getModelviewMatrix(), false );
				DropDownList::program->sendVertexPointer( "a_Vertex", DropDownList::vertices );
				DropDownList::program->sendColorPointer( "a_Color", DropDownList::colors );

				DropDownList::indices->draw( OpenGL::Triangles );
				
				DropDownList::renderingVertices.clear();
				DropDownList::renderingColors.clear();
				DropDownList::renderingIndices.clear();
			}
		}
		
		bool DropDownList::eventPushStateChanged( Element * element, const event::Event * event )
		{
			DropDownList * ddl = reinterpret_cast<DropDownList *>( element );
			
			if( ddl->pushState == true )
			{
				ddl->synchronizeList();
				ddl->value = ddl->title;
				ddl->ui->showElement( ddl->list->getName() );
			}
			else
			{	
				ddl->synchronizeList();
				ddl->ui->hideElement( ddl->list->getName() );
			}
				
			return true;
		}
		
		bool DropDownList::eventListSelectionChanged( Element * element, const event::Event * event )
		{
			List * list = reinterpret_cast<List *>( element );
			DropDownList * ddl = reinterpret_cast<DropDownList *>( list->getUserInterface()->getElement( DropDownList::getButtonName( list->getName() ) ) );
			
			if( ddl != NULL )
			{
				ddl->setPushState( false );
				ddl->trigger( "selectionchanged" );
			}
			
			return true;
		}
	}
}

