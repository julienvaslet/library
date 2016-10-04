#include <blockgame/ScoreSceneEventHandler.h>

namespace blockgame
{
	ScoreSceneEventHandler::ScoreSceneEventHandler( ScoreScene * scene ) : scene(scene)
	{
	}
	
	ScoreSceneEventHandler::~ScoreSceneEventHandler()
	{
	}
	
	void ScoreSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
	{
		if( this->scene != NULL )
		{
			switch( button )
			{
				default:
					break;
					
				case Mapping::NorthButton:
				case Mapping::EastButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				{
					if( value == Mapping::Released )
						this->scene->endScene();
						
					break;
				}
			}
		}
	}
}

