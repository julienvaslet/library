#include <snake/HighscoreScene.h>
#include <snake/HighscoreSceneEventHandler.h>
#include <snake/MenuScene.h>
#include <snake/Constants.h>

#include <opengl/OpenGL.h>
#include <opengl/ui/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>
#include <game/Player.h>
#include <controller/Controller.h>

#include <fstream>
#include <sstream>
using namespace std;
using namespace opengl;
using namespace opengl::ui;
using namespace controller;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace snake
{
	HighscoreScene::HighscoreScene( const string& map, unsigned int score ) : lastDrawTicks(0), lastBlinkTicks(0), score(score), newHighscore(false), shownLetter(true), letter0('A'), letter1(0), letter2(0), map(map)
	{
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new HighscoreSceneEventHandler( this ) );

		if( player == NULL )
		{
			#ifdef DEBUG1
			Logger::get() << "[HighscoreScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
		
		this->loadHighscores();
		
		// Check for new highscore
		if( this->score > 0 )
		{
			for( vector< pair<string, unsigned int> >::iterator it = this->highscores.begin() ; it != this->highscores.end() ; it++ )
			{
				if( this->score > it->second )
				{
					this->newHighscore = true;
					break;
				}
			}
		}
	}
	
	HighscoreScene::~HighscoreScene()
	{
	}
	
	void HighscoreScene::loadHighscores()
	{
		unsigned int entries = 10;
		stringstream filename;
		filename << HIGHSCORES_DIRECTORY << "/" << this->map;
		
		ifstream file( filename.str() );
	
		if( file.is_open() )
		{
			string line;
			
			while( getline( file, line ) )
			{
				unsigned int comma = line.find( ',' );
				
				if( comma != string::npos && comma <= 3 )
				{
					unsigned int entryScore = 0;
					
					string entryName = line.substr( 0, comma );
					istringstream sScore ( line.substr( comma + 1 ) );
					sScore >> entryScore;
					
					if( entryScore > 0 )
					{
						this->highscores.push_back( make_pair( entryName, entryScore ) );
						entries--;
					}
				}
			}
			
			file.close();
		}
		
		while( entries-- > 0 )
			this->highscores.push_back( make_pair( "---", 0 ) );
	}
	
	void HighscoreScene::saveHighscores()
	{
		stringstream filename;
		filename << HIGHSCORES_DIRECTORY << "/" << this->map;
		
		ofstream file( filename.str() );
	
		if( file.is_open() )
		{
			for( vector< pair<string, unsigned int> >::iterator it = this->highscores.begin() ; it != this->highscores.end() ; it++ )
				file << it->first << "," << it->second << std::endl; 
			
			file.close();
		}
	}
	
	void HighscoreScene::handleEvent( SDL_Event * event )
	{
		switch( event->type )
		{
			case SDL_QUIT:
			{
				this->running = false;
				break;
			}
		
			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{
				Controller::handleEvent( event );
				break;
			}
		}
	}
	
	void HighscoreScene::live( unsigned int ticks )
	{
		if( this->newHighscore )
		{
			Controller::tickEvent( ticks );
			
			if( ticks > this->lastBlinkTicks && ticks - this->lastBlinkTicks > 250 )
			{
				this->shownLetter = !this->shownLetter;
				this->lastBlinkTicks = ticks;
			}
		}
	}
	
	void HighscoreScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			stringstream ss;
			
			if( this->newHighscore )
			{
				string title = "New highscore !";
				
				float width = Font::get("bitmap")->getTextWidth( title, 2.0f );
				Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - width) / 2.0f, 100.0f ), title, 2.0f );
				
				string instruction = "Enter your name";
				width = Font::get("bitmap")->getTextWidth( title, 1.5f );
				Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - width) / 2.0f, 210.0f ), instruction, 1.5f );
				
				string letter;
				
				// Letter0
				if( this->letter1 != 0 || ( this->letter1 == 0 && this->shownLetter ) )
				{
					ss.str("");
					ss << this->letter0;
					letter = ss.str();
					Font::get("bitmap")->write( Point2D( 328, 300 ), letter, 1.5f );
				}
				
				// Letter1
				if( this->letter1 != 0 )
				{
					if( this->letter2 != 0 || ( this->letter2 == 0 && this->shownLetter ) )
					{
						ss.str("");
						ss << this->letter1;
						letter = ss.str();
						Font::get("bitmap")->write( Point2D( 376, 300 ), letter, 1.5f );
					}
				
					if( this->letter2 != 0 )
					{
						// Letter2
						if( this->shownLetter )
						{
							ss.str("");
							ss << this->letter2;
							letter = ss.str();
							Font::get("bitmap")->write( Point2D( 424, 300 ), letter, 1.5f );
						}
					}
				}
			}
			else
			{
				// Title
				ss.str("");
				ss << "Highscores - " << this->map;
				string title = ss.str();
			
				float width = Font::get("bitmap")->getTextWidth( title, 1.5f );
				Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - width) / 2.0f, 40.0f ), title, 1.5f );
			
				// Table headers
				Font::get("bitmap")->write( Point2D( 250.0f, 120.0f ), "Name", 1.0f );
				Font::get("bitmap")->write( Point2D( 450.0f, 120.0f ), "Score", 1.0f );
			
				// Table content
				unsigned int iLine = 0;
				for( vector< pair<string, unsigned int> >::const_iterator it = this->highscores.begin() ; it != this->highscores.end() ; it++, iLine++ )
				{
					ss.str("");
					ss << it->second;
					string hscore = ss.str();
				
					Font::get("bitmap")->write( Point2D( 250.0f, 150.0f + iLine * 30.0f ), it->first, 1.0f );
					Font::get("bitmap")->write( Point2D( 450.0f, 150.0f + iLine * 30.0f ), hscore, 1.0f );
				}
			
				ss.str("");
				ss << "Your score: " << this->score;
				string scoreStr = ss.str();
			
				width = Font::get("bitmap")->getTextWidth( scoreStr, 1.0f );
				Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - width) / 2.0f, 500.0f ), scoreStr, 1.0f );
			}
			
			Font::get("bitmap")->render();
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
	
	void HighscoreScene::upAction( unsigned int times )
	{
		if( this->newHighscore && times > 0 )
		{
			char * letter = NULL;
			
			if( this->letter1 == 0 )
				letter = &(this->letter0);
			else if( this->letter2 == 0 )
				letter = &(this->letter1);
			else
				letter = &(this->letter2);
				
			while( times-- > 0 )
			{
				if( *letter == 'A' )
					*letter = 'Z';
				else
					(*letter)--;
			}
			
			this->delayBlinking();
		}
	}
	
	void HighscoreScene::downAction( unsigned int times )
	{
		if( this->newHighscore && times > 0 )
		{
			char * letter = NULL;
			
			if( this->letter1 == 0 )
				letter = &(this->letter0);
			else if( this->letter2 == 0 )
				letter = &(this->letter1);
			else
				letter = &(this->letter2);
			
			while( times-- > 0 )
			{
				if( *letter == 'Z' )
					*letter = 'A';
				else
					(*letter)++;
			}
			
			this->delayBlinking();
		}
	}
	
	void HighscoreScene::buttonAction()
	{
		if( this->newHighscore )
		{
			if( this->letter1 == 0 )
				this->letter1 = 'A';
			else if( this->letter2 == 0 )		
				this->letter2 = 'A';
			else
			{	
				// Highscore validation
				stringstream name;
				name << this->letter0 << this->letter1 << this->letter2;
				vector< pair<string, unsigned int> >::iterator it;

				for( it = this->highscores.begin() ; it != this->highscores.end() ; it++ )
				{
					if( this->score > it->second )
						break;
				}
				
				this->highscores.insert( it, make_pair( name.str(), this->score ) );
				
				while( this->highscores.size() > 10 )
					this->highscores.pop_back();
					
				this->saveHighscores();
				
				this->newHighscore = false;
			}
		}
		else
		{
			this->running = false;
			this->nextScene = new MenuScene();
		}
	}
	
	void HighscoreScene::delayBlinking()
	{
		this->lastBlinkTicks = SDL_GetTicks();
		this->shownLetter = true;
	}
}

