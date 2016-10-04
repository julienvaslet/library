#include <pong/PlaySceneEventHandler.h>

namespace pong
{
	PlaySceneEventHandler::PlaySceneEventHandler( PlayScene * scene, unsigned int player ) : scene(scene), player(player), lastTickEvent(0)
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
					short int verticalAxis = controller->getState( Mapping::VerticalAxis );
					
					if( timestamp - this->lastTickEvent > 10 )
					{
						double times = (timestamp - this->lastTickEvent) / 10.0;
						
						if( verticalAxis == Mapping::Pushed )
						{
							this->scene->moveDown( this->player, times );
						}
						else if( verticalAxis == Mapping::ReversePushed )
						{
							this->scene->moveUp( this->player, times );
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
						this->scene->moveDown( this->player );
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveUp( this->player );
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )
						this->scene->buttonAction( this->player );
						
					break;
				}
			}
		}
	}
}

