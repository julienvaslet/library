#include <controller/Mapping.h>
#include <data/parser/IniParser.h>

#include <string>
#include <fstream>
#include <sstream>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace std;
using namespace data;

#define MAPPINGS_CONFIGURATION_FILE	"data/controllers/mappings.conf"

namespace controller
{
	Mapping::Mapping( const string& name )
	{
		this->load( name );
	}
	
	Mapping::~Mapping()
	{
	}
	
	bool Mapping::load( const string& name )
	{
		map<string, Mapping::Button> internalValues;
		internalValues["NorthButton"] = Mapping::NorthButton;
		internalValues["EastButton"] = Mapping::EastButton;
		internalValues["SouthButton"] = Mapping::SouthButton;
		internalValues["WestButton"] = Mapping::WestButton;
		internalValues["LeftTrigger"] = Mapping::LeftTrigger;
		internalValues["RightTrigger"] = Mapping::RightTrigger;
		internalValues["SelectButton"] = Mapping::SelectButton;
		internalValues["StartButton"] = Mapping::StartButton;
		internalValues["HorizontalAxis"] = Mapping::HorizontalAxis;
		internalValues["VerticalAxis"] = Mapping::VerticalAxis;
		
		ifstream iniFile( MAPPINGS_CONFIGURATION_FILE );

		if( iniFile.is_open() )
		{
			stringstream ss;
			ss << iniFile.rdbuf();
			iniFile.close();
	
			parser::IniParser * parser = new parser::IniParser( ss.str() );
			ini::Configuration * conf = parser->parse();
			delete parser;
	
			if( conf->hasSection( name ) )
			{
				#ifdef DEBUG0
				Logger::get() << "[Mapping][" << name << "] Loading mapping..." << Logger::endl;
				#endif
				
				for( map<string, Mapping::Button>::const_iterator it = internalValues.begin() ; it != internalValues.end() ; it++ )
				{
					if( conf->hasKey( it->first, name ) )
					{
						int iValue = 0;
						istringstream iss( conf->getValue( it->first, name ) );
	
						if( iss )
							iss >> iValue;
						
						if( internalValues[it->first] == Mapping::HorizontalAxis || internalValues[it->first] == Mapping::VerticalAxis )
						{
							this->axes[iValue] = internalValues[it->first];
							
							#ifdef DEBUG0
							Logger::get() << "[Mapping][" << name << "][Axis] " << it->first << " = " << iValue << Logger::endl;
							#endif
						}
						else
						{
							this->buttons[iValue] = internalValues[it->first];
							
							#ifdef DEBUG0
							Logger::get() << "[Mapping][" << name << "][Button] " << it->first << " = " << iValue << Logger::endl;
							#endif
						}
					}
				}
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[Mapping] No mapping for \"" << name << "\"." << Logger::endl;
			#endif
			
			delete conf;
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Mapping] Unable to open file " MAPPINGS_CONFIGURATION_FILE "." << Logger::endl;
		#endif
		
		return this->isLoaded();
	}
	
	bool Mapping::isLoaded()
	{
		return this->buttons.size() + this->axes.size() > 0;
	}

	Mapping::Button Mapping::getButtonFromButton( int value )
	{
		Mapping::Button btn = Mapping::NoButton;
		
		map<int, Mapping::Button>::iterator it = this->buttons.find( value );
		
		if( it != this->buttons.end() )
			btn = it->second;
		
		return btn;
	}
	
	Mapping::Button Mapping::getButtonFromAxis( int value )
	{
		Mapping::Button btn = Mapping::NoButton;
		
		map<int, Mapping::Button>::iterator it = this->axes.find( value );
		
		if( it != this->axes.end() )
			btn = it->second;
		
		return btn;
	}
}

