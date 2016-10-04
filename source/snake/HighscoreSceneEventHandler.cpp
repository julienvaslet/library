#include <snake/HighscoreSceneEventHandler.h>

namespace snake
{
	HighscoreSceneEventHandler::HighscoreSceneEventHandler( HighscoreScene * scene ) : scene(scene), lastTickEvent(0)
	{
	}
	
	HighscoreSceneEventHandler::~HighscoreSceneEventHandler()
	{
	}
	
	void HighscoreSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
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
					
					if( timestamp > this->lastTickEvent && timestamp - this->lastTickEvent > 150 )
					{
						double times = (timestamp - this->lastTickEvent) / 150.0;
						
						if( verticalAxis == Mapping::Pushed )
						{
							this->scene->downAction( times );
						}
						else if( verticalAxis == Mapping::ReversePushed )
						{
							this->scene->upAction( times );
						}
						
						this->lastTickEvent = timestamp;
					}
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					this->lastTickEvent = timestamp + 250;
					
					if( value == Mapping::Pushed )
					{
						this->scene->downAction();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->upAction();
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )	
						this->scene->buttonAction();
						
					break;
				}
			}
		}
	}
}

