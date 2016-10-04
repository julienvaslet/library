#include <tools/logger/Stdout.h>
#include <data/script/Script.h>
#include <string>
#include <queue>

using namespace tools::logger;
using namespace data::script;
using namespace std;

#define GREEN	"\033[0;32m"
#define RED		"\033[0;31m"
#define WHITE	"\033[0m"

#define TEST( name, source, expectedStatus, validation )	\
tests++;\
script->setSource( source, name );\
status = script->run( true );\
if( status == expectedStatus && validation )\
{\
	Logger::get() << "[Test#" name "] " GREEN "Passed." WHITE << Logger::endl;\
	successfulTests++;\
}\
else\
	Logger::get() << "[Test#" name "] " RED "Failed!" WHITE << Logger::endl;\

Json * getVariable( Script * script, const string& variable );
bool variableExists( Script * script, const string& variable );
bool isNullValue( Script * script, const string& variable );

bool checkValue( Script * script, const string& variable, int value );
bool checkValue( Script * script, const string& variable, double value );
bool checkValue( Script * script, const string& variable, bool value );
bool checkValue( Script * script, const string& variable, const char * value );

bool checkArrayLength( Script * script, const string& variable, unsigned int length );
bool checkObjectLength( Script * script, const string& variable, unsigned int length );


int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	bool status = true;
	unsigned int tests = 0;
	unsigned int successfulTests = 0;
	Script * script = new Script( "" );
	
	// Variable initialization
	TEST( "SimpleVariableInitialization", "var variable;", true, (isNullValue(script, "variable")) );
	TEST( "ImplicitVariableInitialization", "variable = 2;", true, (checkValue(script, "variable", 2)) );
	TEST( "PositiveIntegerVariableInitialization", "var variable = 7;", true, (checkValue(script, "variable", 7)) );
	TEST( "NegativeIntegerVariableInitialization", "var variable = -7;", true, (checkValue(script, "variable", -7)) );
	TEST( "DecimalVariableInitialization", "var variable = 3.14;", true, (checkValue(script, "variable", 3.14)) );
	TEST( "NullVariableInitialization", "var variable = null;", true, (isNullValue(script, "variable")) );
	TEST( "BooleanTrueVariableInitialization", "var variable = true;", true, (checkValue(script, "variable", true)) );
	TEST( "BooleanFalseVariableInitialization", "var variable = false;", true, (checkValue(script, "variable", false)) );
	TEST( "StringVariableInitialization", "var variable = \"Hello\";", true, (checkValue(script, "variable", "Hello")) );
	TEST( "ArrayVariableInitialization", "var variable = [ 1, 2 ];", true, (checkArrayLength(script, "variable", 2 ) && checkValue(script, "variable[0]", 1) && checkValue(script, "variable[1]", 2)) );
	TEST( "EmptyArrayVariableInitialization", "var variable = [];", true, (checkArrayLength(script, "variable", 0 )) );
	TEST( "ObjectVariableInitialization", "var variable = { \"a\": 1, \"b\": 2 };", true, (checkValue(script, "variable.a", 1) && checkValue(script, "variable.b", 2)) );
	TEST( "EmptyObjectVariableInitialization", "var variable = {};", true, (checkObjectLength(script, "variable", 0 )) );
	
	// Malformed variable initialization
	TEST( "MissingSemicolonOnVariableInitialization", "var variable = 4", false, (true) );
	TEST( "MissingValueOnVariableInitialization", "var variable = ;", false, (true) );
	TEST( "MissingOpeningBracketOnArrayInitialization", "var variable = 1, 2 ];", false, (true) );
	TEST( "MissingClosingBracketOnArrayInitialization", "var variable = [ 1, 2 ;", false, (true) );
	TEST( "MissingCommaOnArrayInitialization", "var variable = [ 1 2 ];", false, (true) );
	TEST( "MissingOpeningBraceOnObjectInitialization", "var variable = \"element\": 1, \"value\": true };", false, (true) );
	TEST( "MissingClosingBraceOnObjectInitialization", "var variable = { \"element\": 1, \"value\": true ;", false, (true) );
	TEST( "MissingCommaOnObjectInitialization", "var variable = { \"element\": 1 \"value\": true };", false, (true) );
	TEST( "MissingColonOnObjectInitialization", "var variable = { \"element\" 1, \"value\": true };", false, (true) );
	
	// Variable modification
	TEST( "ArrayElementModification", "var variable = [ 1 ]; variable[0] = 2;", true, (checkValue(script, "variable[0]", 2)) );
	TEST( "ObjectElementModification", "var variable = { \"a\": 1 }; variable.a = 2;", true, (checkValue(script, "variable.a", 2)) );
	
	delete script;
	
	Logger::get()	<< "Summary" << Logger::endl
					<< "=======" << Logger::endl
					<< "Tests:    " << tests << Logger::endl
					<< "Success:  " << successfulTests << Logger::endl;
	
	Logger::destroy();
	
	return 0;
}

Json * getVariable( Script * script, const string& variable )
{
	Json * json = NULL;
	DOM * dom = script->getDOM();
	
	queue<string> names;
	queue<int> indexes;
	
	if( dom != NULL )
	{
		unsigned int currentPosition = 0;
		
		do
		{
			unsigned int dot = variable.find( '.', currentPosition );
			unsigned int bracket = variable.find( '[', currentPosition );
			
			// Dot case
			if( dot != string::npos && ( bracket == string::npos || dot < bracket ) )
			{
				names.push( variable.substr( currentPosition, dot - currentPosition ) );
				currentPosition = dot + 1;
				
				indexes.push( -1 );
			}
			
			// Bracket case
			else if( bracket != string::npos && ( dot == string::npos || bracket < dot ) )
			{
				bool bracketSuccess = false;
				
				names.push( variable.substr( currentPosition, bracket - currentPosition ) );
				currentPosition = bracket + 1;
				
				bracket = variable.find( ']', currentPosition );
				
				if( bracket != string::npos )
				{
					string index = variable.substr( currentPosition, bracket - currentPosition );
					currentPosition = bracket + 1;
					
					try
					{
						indexes.push( stoi( index ) );
						bracketSuccess = true;
					}
					catch( const exception& e )
					{
					}
				}
				
				if( !bracketSuccess )
				{
					while( !names.empty() ) names.pop();
					while( !indexes.empty() ) indexes.pop();
					
					currentPosition = variable.length();
				}
			}
			
			// End of expression case
			else
			{
				names.push( variable.substr( currentPosition ) );
				currentPosition = variable.length();
				
				indexes.push( -1 );
			}
		}
		while( currentPosition < variable.length() );
		
		if( !names.empty() )
		{
			while( !names.empty() )
			{
				if( json == NULL )
				{
					try
					{
						json = dom->get( names.front() );
					}
					catch( const std::string& str )
					{
						json = NULL;
						break;
					}
				}
				else
				{
					if( json->getType() == Json::Object )
					{
						Object * o = json->asObject();
						if( o->hasKey( names.front() ) )
						{
							json = (*o)[names.front()];
						}
						else
						{
							json = NULL;
							break;
						}
					}
					else
					{
						json = NULL;
						break;
					}
				}
				
				if( indexes.front() >= 0 )
				{
					if( json->getType() == Json::Array )
					{
						Array * a = json->asArray();
						
						if( static_cast<unsigned int>( indexes.front() ) < a->length() )
						{
							json = a->get( indexes.front() );
						}
						else
						{
							json = NULL;
							break;
						}
					}
					else
					{
						json = NULL;
						break;
					}
				}
				
				names.pop();
				indexes.pop();
			}
		}
	}
	
	return json;
}

bool variableExists( Script * script, const string& variable )
{
	return getVariable( script, variable ) != NULL;
}

bool isNullValue( Script * script, const string& variable )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
	
	if( json != NULL && json->getType() == Json::Null )
		bReturn = true;
	
	return bReturn;
}

bool checkValue( Script * script, const string& variable, int value )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		Number * number = json->asNumber();
		
		if( number != NULL )
		{
			if( number->getValue() == static_cast<double>( value ) )
				bReturn = true;
		}
	}
	
	return bReturn;
}

bool checkValue( Script * script, const string& variable, double value )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		Number * number = json->asNumber();
		
		if( number != NULL )
		{
			if( number->getValue() == value )
				bReturn = true;
		}
	}
	
	return bReturn;
}

bool checkValue( Script * script, const string& variable, bool value )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		Boolean * boolean = json->asBoolean();
		
		if( boolean != NULL )
		{
			if( boolean->getValue() == value )
				bReturn = true;
		}
	}
	
	return bReturn;
}

bool checkValue( Script * script, const string& variable, const char * value )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		String * text = json->asString();
		
		if( text != NULL )
		{	
			if( text->getValue() == value )
				bReturn = true;
		}
	}
	
	return bReturn;
}

bool checkArrayLength( Script * script, const string& variable, unsigned int length )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		Array * array = json->asArray();
		
		if( array != NULL )
		{
			if( array->length() == length )
				bReturn = true;
		}
	}
	
	return bReturn;
}

bool checkObjectLength( Script * script, const string& variable, unsigned int length )
{
	bool bReturn = false;
	Json * json = getVariable( script, variable );
		
	if( json != NULL )
	{
		Object * object = json->asObject();
		
		if( object != NULL )
		{
			if( object->length() == length )
				bReturn = true;
		}
	}
	
	return bReturn;
}

