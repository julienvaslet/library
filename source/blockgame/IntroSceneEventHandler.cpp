#include <blockgame/IntroSceneEventHandler.h>

namespace blockgame
{
	IntroSceneEventHandler::IntroSceneEventHandler( IntroScene * scene ) : scene(scene)
	{
	}
	
	IntroSceneEventHandler::~IntroSceneEventHandler()
	{
	}
	
	void IntroSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
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

