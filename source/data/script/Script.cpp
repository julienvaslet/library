#include <data/script/Script.h>
#include <sstream>
#include <fstream>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace script
	{
		Script::Script( const string& code ) : dom(NULL), interpreter(NULL)
		{
			this->dom = new DOM( NULL );
			this->interpreter = new ScriptInterpreter( code, this );
		}
		
		Script::Script( const string& code, const string& name ) : dom(NULL), interpreter(NULL)
		{
			this->dom = new DOM( NULL );
			this->interpreter = new ScriptInterpreter( code, this, name );
		}
		
		Script::~Script()
		{
			if( this->interpreter != NULL )
				delete this->interpreter;
				
			if( this->dom != NULL )
				delete this->dom;
		}
		
		void Script::setSource( const string& code )
		{
			if( this->interpreter != NULL )
				delete this->interpreter;
				
			this->interpreter = new ScriptInterpreter( code, this );
		}
		
		void Script::setSource( const string& code, const string& name )
		{
			if( this->interpreter != NULL )
				delete this->interpreter;
				
			this->interpreter = new ScriptInterpreter( code, this, name );
		}
		
		DOM * Script::getDOM()
		{
			return this->dom;
		}
	
		bool Script::run( bool silent )
		{
			bool bReturn = this->interpreter->run( false, silent );
			
			return bReturn;
		}
		
		bool Script::check()
		{
			return this->interpreter->run( true );
		}
		
		string Script::getSource()
		{
			return this->interpreter->getParsedSource( true );
		}
		
		Script * Script::load( const string& filename )
		{
			Script * script = NULL;
			string scriptName;
			stringstream source;
			ifstream scriptFile( filename.c_str() );
			
			unsigned int lastSlash = filename.find_last_of( '/' );
			
			if( lastSlash == string::npos || lastSlash + 1 == filename.length() )
				scriptName = filename;
			else
				scriptName = filename.substr( lastSlash + 1 );
	
			if( scriptFile.is_open() )
			{
				source << scriptFile.rdbuf();
				scriptFile.close();
		
				script = new Script( source.str(), scriptName );
			}
			#ifdef DEBUG1
			else
				Logger::get() << "[Script][" << scriptName << "] Unable to open file \"" << filename << "\"." << Logger::endl;
			#endif
			
			return script;
		}
	}
}

