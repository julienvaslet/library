#include <snake/MenuSceneEventHandler.h>

namespace snake
{
	MenuSceneEventHandler::MenuSceneEventHandler( MenuScene * scene ) : scene(scene), lastTickEvent(0)
	{
	}
	
	MenuSceneEventHandler::~MenuSceneEventHandler()
	{
	}
	
	void MenuSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
	{
		if( this->scene != NULL )
		{
			switch( button )
			{		
				case Mapping::NoButton:
				{
					// Tick event
					short int verticalAxis = controller->getState( Mapping::VerticalAxis );
					
					if( timestamp - this->lastTickEvent > 10 )
					{
						double times = (timestamp - this->lastTickEvent) / 10.0;
						
						if( verticalAxis == Mapping::Pushed )
						{
							this->scene->selectLowerAction( times );
						}
						else if( verticalAxis == Mapping::ReversePushed )
						{
							this->scene->selectUpperAction( times );
						}
						
						this->lastTickEvent = timestamp;
					}
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					this->lastTickEvent = timestamp;
					
					if( value == Mapping::Pushed )
					{
						this->scene->selectLowerAction();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->selectUpperAction();
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )
						this->scene->selectCurrentAction();

					break;
				}
				
				default:
					break;
			}
		}
	}
}

