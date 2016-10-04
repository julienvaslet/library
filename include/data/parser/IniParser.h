#ifndef __DATA_PARSER_INIPARSER_H
#define __DATA_PARSER_INIPARSER_H	1

#include <data/parser/Parser.h>
#include <data/ini/Configuration.h>

using namespace std;

namespace data
{
	namespace parser
	{
		class IniParser : public Parser
		{
			protected:
				ini::Configuration * configuration;
				ini::Section * currentSection;
			
				bool section();
				bool comment();
				bool assignation();
				bool emptyline();
	
			public:
				IniParser( const string& content );
				~IniParser();
		
				ini::Configuration * parse();
		};
	}
}

#endif
