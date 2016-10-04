#include <overscan/ConfigSceneEventHandler.h>

namespace overscan
{
	ConfigSceneEventHandler::ConfigSceneEventHandler( ConfigScene * scene ) : scene(scene), lastTickEvent(0)
	{
	}
	
	ConfigSceneEventHandler::~ConfigSceneEventHandler()
	{
	}
	
	void ConfigSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
	{
		if( this->scene != NULL )
		{
			switch( button )
			{
				default:
					break;
					
				case Mapping::NoButton:
				{
					// Tick event
					short int verticalAxis = controller->getState( Mapping::VerticalAxis );
					short int horizontalAxis = controller->getState( Mapping::HorizontalAxis );
					
					if( timestamp - this->lastTickEvent > 250 )
					{
						unsigned int times = static_cast<unsigned int>( floor( (timestamp - this->lastTickEvent) / 250.0 ) );
						
						if( verticalAxis == Mapping::Pushed )
							this->scene->moveDown( times );
						else if( verticalAxis == Mapping::ReversePushed )
							this->scene->moveUp( times );
							
						if( horizontalAxis == Mapping::Pushed )
							this->scene->moveRight( times );
						else if( horizontalAxis == Mapping::ReversePushed )
							this->scene->moveLeft( times );
						
						this->lastTickEvent = timestamp;
					}
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					this->lastTickEvent = timestamp;
					
					if( value == Mapping::Pushed )
					{
						this->scene->moveDown();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveUp();
					}
						
					break;
				}
				
				case Mapping::HorizontalAxis:
				{
					this->lastTickEvent = timestamp;
					
					if( value == Mapping::Pushed )
					{
						this->scene->moveRight();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveLeft();
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )
						this->scene->nextStep();
						
					break;
				}
			}
		}
	}
}

