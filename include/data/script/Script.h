#ifndef __DATA__SCRIPT__SCRIPT_H
#define __DATA__SCRIPT__SCRIPT_H	1

namespace data
{
	namespace script
	{
		class Script;
	}
}

#include <data/parser/ScriptInterpreter.h>
#include <data/json/DOM.h>
#include <string>

using namespace std;
using namespace data::json;
using namespace data::parser;

namespace data
{
	namespace script
	{
		class Script
		{
			friend ScriptInterpreter;
			
			protected:
				DOM * dom;
				ScriptInterpreter * interpreter;
				
			public:
				Script( const string& code );
				Script( const string& code, const string& name );
				virtual ~Script();
				
				bool run( bool silent = false );
				bool check();
				DOM * getDOM();
				string getSource();
				void setSource( const string& code );
				void setSource( const string& code, const string& name );
				
				static Script * load( const string& filename );
		};
	}
}

#endif

