#include <opengl/ui/UserInterface.h>
#include <opengl/Screen.h>
#include <opengl/ui/event/Event.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		UserInterface * UserInterface::userInterface = NULL;
		
		UserInterface::UserInterface( unsigned int width, unsigned int height ) : fontName(OPENGL_UI_DEFAULT_FONT), fontSize(0.0f), width(width), height(height)
		{
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Created." << Logger::endl;
			#endif
			
			if( this->width == 0 || this->height == 0 )
			{
				this->width = Screen::get()->getWidth();
				this->height = Screen::get()->getHeight();
			}
			
			this->resize( this->width, this->height );
		}
		
		UserInterface::~UserInterface()
		{
			for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
				delete it->second;
			
			this->elements.clear();
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Deleted." << Logger::endl;
			#endif
		}
		
		bool UserInterface::create( unsigned int width, unsigned int height )
		{
			if( UserInterface::userInterface == NULL )
			{
				UserInterface::userInterface = new UserInterface( width, height );
				return true;
			}
			else
				return false;
		}
		
		UserInterface * UserInterface::get()
		{
			if( UserInterface::userInterface == NULL )
				UserInterface::create();
				
			return UserInterface::userInterface;
		}
		
		void UserInterface::resize( unsigned int width, unsigned int height )
		{
			this->width = width;
			this->height = height;
			
			this->projection = Matrix::ortho( 0, this->width, this->height, 0, -10, 10 );
			this->modelview = Matrix::identity();
		}
		
		const Matrix& UserInterface::getProjectionMatrix()
		{
			return this->projection;
		}
		
		const Matrix& UserInterface::getModelviewMatrix()
		{
			return this->modelview;
		}
		
		void UserInterface::setFont( const string& font, float fontSize )
		{
			this->fontName = font;
			this->fontSize = fontSize;
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Interface font is \"" << this->fontName << "\" (size: " << this->fontSize << ")" << Logger::endl;
			
			if( this->getFont() == NULL )
				Logger::get() << "[UserInterface] /!\\ Font \"" << this->fontName << "\" seems to not be loaded." << Logger::endl;
			#endif
		}
		
		float UserInterface::getFontSize()
		{
			return this->fontSize;
		}
		
		Font * UserInterface::getFont()
		{
			return Font::get( this->fontName );
		}
		
		void UserInterface::addElement( Element * element, bool hidden )
		{
			map<string, Element *>::iterator it = this->elements.find( element->getName() );
			
			if( it != this->elements.end() )
			{
				#ifdef DEBUG0
				Logger::get() << "[UserInterface] /!\\ Element#" << element->getName() << " will be replaced by a new one." << Logger::endl;
				#endif
				
				delete it->second;
			}
			
			this->elements[element->getName()] = element;
			element->setUserInterface( this );
			
			if( hidden )
				this->hideElement( element->getName() );
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Element#" << element->getName() << " has been added." << Logger::endl;
			#endif
		}
		
		bool UserInterface::hasElement( const string& name )
		{
			map<string, Element *>::iterator it = this->elements.find( name );
			return ( it != this->elements.end() );
		}
		
		Element * UserInterface::getElement( const string& name )
		{
			Element * element = NULL;
			
			map<string, Element *>::iterator it = this->elements.find( name );
			
			if( it != this->elements.end() )
				element = it->second;
			
			return element;
		}
		
		void UserInterface::removeElement( const string& name, bool deleteElement )
		{
			map<string, Element *>::iterator it = this->elements.find( name );
			
			if( it != this->elements.end() )
			{
				if( it->second->getUserInterface() == this )
				{
					#ifdef DEBUG0
					Logger::get() << "[UserInterface] Element#" << name << " has been removed." << Logger::endl;
					#endif
					
					if( deleteElement )
						delete it->second;
						
					this->elements.erase( it );
				}
				#ifdef DEBUG0
				else
				{
					Logger::get() << "[UserInterface] /!\\ Element \"" << name << "\" asked to be removed, but it does not belong to current UserInterace." << Logger::endl;
				}
				#endif
			}
		}
		
		void UserInterface::clear()
		{
			for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
			{
				if( it->second->getUserInterface() == this )
					delete it->second;
			}
			
			this->elements.clear();
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] All elements have been removed." << Logger::endl;
			#endif
		}

		void UserInterface::showElement( const string& name )
		{
			this->hiddenElements.erase( name );
		}
		
		void UserInterface::hideElement( const string& name )
		{
			this->hiddenElements.insert( name );
		}
		
		void UserInterface::render( unsigned int ticks )
		{
			// Dock UserInterface to the top-left window corner
			glViewport( 0, Screen::get()->getHeight() - this->height, this->width, this->height );
			
			for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
			{
				if( this->hiddenElements.count( it->first ) == 0 )
					it->second->prepareRendering( ticks );
			}
				
			Element::render( ticks );
			
			this->getFont()->render();
		}
		
		bool UserInterface::dispatchEvent( const SDL_Event * event )
		{
			bool eventHandled = false;
			
			event::Event * e = event::Event::create( event );
			
			if( e != NULL )
			{
				switch( e->getType() )
				{
					case event::EventType::Mouse:
					{
						event::MouseEvent * mEvent = static_cast<event::MouseEvent *>( e );
						Element * targetedElement = NULL;
						
						for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
						{
							if( this->hiddenElements.count( it->first ) == 0 && !it->second->isDisabled() )
							{
								if( it->second->getRectangle().isInCollision( mEvent->getOrigin() ) )
								{
									if( targetedElement == NULL || targetedElement->getZ() < it->second->getZ() )
										targetedElement = it->second;
								}
								else if( this->mouseoverElements.count( it->first ) > 0 )
								{
									this->mouseoverElements.erase( it->first );
									
									event::MouseEvent * leaveEvent = new event::MouseEvent( "mouseleave", mEvent );
									it->second->trigger( leaveEvent );
									delete leaveEvent;
								}
							}
						}
						
						if( targetedElement != NULL )
						{
							targetedElement->trigger( mEvent );
							eventHandled = true;
							
							if( this->mouseoverElements.count( targetedElement->getName() ) == 0 )
							{
								this->mouseoverElements.insert( targetedElement->getName() );
								
								event::MouseEvent * enterEvent = new event::MouseEvent( "mouseenter", mEvent );
								targetedElement->trigger( enterEvent );
								delete enterEvent;
							}
						}
						
						break;
					}
					
					case event::EventType::Window:
					{
						break;
					}
					
					case event::EventType::Keyboard:
					{
						break;
					}
					
					case event::EventType::Action:
					{
						break;
					}
				}
				
				delete e;
			}
			
			return eventHandled;
		}
	}
}

