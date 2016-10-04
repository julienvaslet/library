#ifndef __DATA__PARSER__SCRIPT_INTERPRETER_H
#define __DATA__PARSER__SCRIPT_INTERPRETER_H	1

#include <data/parser/Parser.h>
#include <data/json/Json.h>

namespace data
{
	namespace parser
	{
		class ScriptInterpreter;
	}
}

#include <data/script/Script.h>

using namespace data::script;

namespace data
{
	namespace parser
	{
		class ScriptInterpreter : public Parser
		{
			protected:
				static unsigned int noNameScriptIncrement;
				
				string scriptName;
				Script * script;
				json::Json * currentValue;
				stack<json::Json *> stackedValues;
				string currentName;
				stack<string> stackedNames;
				vector<string> reservedNames;
				
				bool noop;
				bool aborted;
				bool silent;
			
				const string& getScriptName();
				
				void initStacks();
				void pushValue();
				void popValue( bool setActive = true );
				void pushName();
				void popName( bool setActive = true );
				
				void handleError( const string& message, bool abort = false );
			
				bool assignation();
				bool expression();
				
				/* Values */
				bool value();
				
				bool name();
				bool number();
				bool null();
				bool boolean();
				bool text();
				bool array();
				bool object();
				
				bool operation();
				bool sum();
				bool substract();
				bool divide();
				bool multiply();
	
			public:
				ScriptInterpreter( const string& content, Script * script, const string& name = "" );
				~ScriptInterpreter();
		
				bool run( bool noop = false, bool silent = false );
		};
	}
}

#endif
