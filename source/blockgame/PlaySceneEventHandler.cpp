#include <blockgame/PlaySceneEventHandler.h>
#include <iostream>

namespace blockgame
{
	PlaySceneEventHandler::PlaySceneEventHandler( PlayScene * scene ) : scene(scene)
	{
	}
	
	PlaySceneEventHandler::~PlaySceneEventHandler()
	{
	}
	
	void PlaySceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
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
					unsigned int stateTimestamp = controller->getStateTimestamp( Mapping::HorizontalAxis );
					short int horizontalAxis = controller->getState( Mapping::HorizontalAxis );
					
					if( timestamp - stateTimestamp > 100 )
					{
						if( horizontalAxis == Mapping::Pushed )
						{
							this->scene->moveRight();
						}
						else if( horizontalAxis == Mapping::ReversePushed )
						{
							this->scene->moveLeft();
						}
					}
					
					break;
				}
				
				case Mapping::HorizontalAxis:
				{
					if( value >= Mapping::Pushed )
						this->scene->moveRight();
						
					else if( value <= Mapping::ReversePushed )
						this->scene->moveLeft();
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					if( value >= Mapping::Pushed )
						this->scene->moveDown();
						
					break;
				}
				
				case Mapping::LeftTrigger:
				case Mapping::WestButton:
				{
					if( value == Mapping::Pushed )
						this->scene->rotate( false );
						
					break;
				}
				
				case Mapping::RightTrigger:
				case Mapping::EastButton:
				{
					if( value == Mapping::Pushed )
						this->scene->rotate( true );
						
					break;
				}
			}
		}
	}
}

