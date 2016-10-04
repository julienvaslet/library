#ifndef __CONTROLLER_MAPPING_H
#define __CONTROLLER_MAPPING_H	1

#include <string>
#include <map>
using namespace std;

namespace controller
{
	class Mapping
	{
		public:
			enum Button
			{
				NoButton,
				NorthButton,
				EastButton,
				SouthButton,
				WestButton,
				LeftTrigger,
				RightTrigger,
				SelectButton,
				StartButton,
				HorizontalAxis,
				VerticalAxis
			};
			
			enum State
			{
				ReversePushed	= -16384,
				Pushed			= 16384,
				Released		= 0
			};
		
		protected:
			map<int, Button> buttons;
			map<int, Button> axes;
			
			bool load( const string& name );
		
		public:
			Mapping( const string& name );
			~Mapping();
			
			bool isLoaded();
			
			Mapping::Button getButtonFromButton( int value );
			Mapping::Button getButtonFromAxis( int value );
	};
}

#endif

