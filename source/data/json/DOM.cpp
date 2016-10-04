#include <data/json/DOM.h>

#include <sstream>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace json
	{
		DOM::DOM( Object * object )
		{
			#ifdef DEBUG0
			Logger::get() << "[Json][DOM] Initialized new DOM" << Logger::endl;
			#endif
			
			if( object != NULL )
			{
				vector<string> keys = object->keys();
				
				for( vector<string>::iterator it = keys.begin() ; it != keys.end() ; it++ )
					this->set( *it, (*object)[ *it ] );
			}
		}
		
		DOM::~DOM()
		{
			#ifdef DEBUG0
			Logger::get() << "[Json][DOM] Cleaning DOM Objects..." << Logger::endl;
			#endif
			
			for( map<string, Json *>::const_iterator it = this->variables.begin() ; it != this->variables.end() ; it++ )
				delete it->second;
			
			#ifdef DEBUG0
			Logger::get() << "[Json][DOM] Deleted." << Logger::endl;
			#endif
		}
		
		/*void DOM::free( Json * value )
		{
			if( value != NULL )
			{
				// Deleting array automatically deletes its sons
				//if( value->getType() == Json::Array )
				//{
				//	json::Array * array = static_cast<json::Array *>( value );
					
				//	while( array->length() > 0 )
				//		DOM::free( array->pop() );
				//}
				
				delete value;
			}
		}*/
		
		void DOM::free( const string& path )
		{
			/*if( value != NULL )
			{
				
				delete value;
			}*/
		}
		
		bool DOM::pathAnalysis( const string& path, queue<string>& names, queue<int>& indices )
		{
			unsigned int currentPosition = 0;
			
			do
			{
				unsigned int dot = path.find( '.', currentPosition );
				unsigned int bracket = path.find( '[', currentPosition );
			
				// Dot case
				if( dot != string::npos && ( bracket == string::npos || dot < bracket ) )
				{
					names.push( path.substr( currentPosition, dot - currentPosition ) );
					currentPosition = dot + 1;
				
					indices.push( -1 );
				}
			
				// Bracket case
				else if( bracket != string::npos && ( dot == string::npos || bracket < dot ) )
				{
					bool bracketSuccess = false;
				
					names.push( path.substr( currentPosition, bracket - currentPosition ) );
					currentPosition = bracket + 1;
				
					bracket = path.find( ']', currentPosition );
				
					if( bracket != string::npos )
					{
						string index = path.substr( currentPosition, bracket - currentPosition );
						currentPosition = bracket + 1;
					
						try
						{
							indices.push( stoi( index ) );
							bracketSuccess = true;
						}
						catch( const exception& e )
						{
						}
					}
				
					if( !bracketSuccess )
					{
						while( !names.empty() ) names.pop();
						while( !indices.empty() ) indices.pop();
					
						currentPosition = path.length();
					}
				}
			
				// End of expression case
				else
				{
					names.push( path.substr( currentPosition ) );
					currentPosition = path.length();
				
					indices.push( -1 );
				}
			}
			while( currentPosition < path.length() );
			
			return !names.empty();
		}
		
		bool DOM::set( const string& path, Json * value )
		{
			bool bReturn = false;
			
			if( value->getDOM() == NULL )
			{
				value->setDOM( this );
			}
			else if( value->getDOM() != this )
			{
				value = value->clone();
				value->setDOM( this );
			}
			
			queue<string> names;
			queue<int> indices;
			this->pathAnalysis( path, names, indices );

			ostringstream currentPath;
			string currentName;
			int currentIndex;
			
			// TODO: complete set() fctionality
			while( !names.empty() )
			{
				currentName = names.front();
				currentIndex = indices.front();
				
				names.pop();
				indices.pop();
				
				currentPath << currentName;
				
				// Get the value from the DOM
				if( previousValue == NULL )
				{
					map<string, Json *>::iterator it = this->variables.find( names.front() );
					
					if( it != this->variable.end() )
						previousValue = it->second;
				}
				
				// Array's index management
				if( indices.front() > -1 )
				{
					if( previousValue == NULL || previousValue->getType() != Json::Array )
					{
						Logger::get() << "[DOM] Unable to access element: \"" << currentPath.str() << "\" is not an array." << Logger::endl;
						break;
					}
					
					Array * array = previousValue->asArray();
					
					if( indices.front() < array->length() )
					{
						currentPath << "[" << indices.front() << "]";
						previousValue = previousValue.at( indices.front() );
					}
					else
					{
						Logger::get() << "[DOM] Specified index (" << indices.front() << ") is out of array (\"" << currentPath.str() << "\")." << Logger::endl;
						break;
					}
				}
			}
			
			this->variables[ path ] = value;
			
			return bReturn;
		}
		
		bool DOM::set( const string& path, const string& value )
		{
			return this->set( path, json::String::create( value, this ) );
		}
		
		bool DOM::set( const string& path, const char * value )
		{
			return this->set( path, json::String::create( value, this ) );
		}
		
		bool DOM::set( const string& path, int value )
		{
			return this->set( path, json::Number::create( static_cast<double>( value ), this ) );
		}
		
		bool DOM::set( const string& path, double value )
		{
			return this->set( path, json::Number::create( value, this ) );
		}
		
		bool DOM::set( const string& path, bool value )
		{
			return this->set( path, json::Boolean::create( value, this ) );
		}

		Json * DOM::get( const string& path )
		{
			vector<string> names;
			unsigned int lastDot = 0;
			unsigned int dot = 0;
			
			do
			{
				dot = path.find( ".", lastDot );
				
				if( dot == string::npos )
					dot = path.length();
				
				names.push_back( path.substr( lastDot, dot - lastDot ) );
					
				lastDot = dot + 1;
			}
			while( dot < path.length() );
			
			Json * element = NULL;
			vector<string>::iterator name = names.begin();
			
			if( name != names.end() && name->length() > 0 )
			{
				if( this->variables.find( *name ) != this->variables.end() )
				{
					element = this->variables[ *name ];
					
					while( ++name != names.end() )
					{
						if( element->getType() == Json::Object )
						{
							Object * obj = static_cast<Object *>( element );
							
							if( obj->hasKey( *name ) )
							{
								element = (*obj)[ *name ];
							}
							else
							{
								element = NULL;
							
								ostringstream err;
								err << "Variable \"" << *(--name) << "\" has no \"";
								err << *(++name) << "\" key.";
								throw err.str();
							}
						}
						else
						{
							element = NULL;
							
							ostringstream err;
							err << "Variable \"" << *(--name) << "\" is not an object.";
							throw err.str();
						}
					}
				}
				else
				{
					ostringstream err;
					err << "Variable \"" << *name << "\" is not defined.";
					throw err.str();
				}
			}
			else
			{
				throw string( "An empty path was specified." );
			}
			
			return element;
		}
		
		string DOM::toString() const
		{
			ostringstream text;
			
			for( map<string, Json *>::const_iterator it = this->variables.begin() ; it != this->variables.end() ; it++ )
			{
				text << "----" << endl;
				text << "Variable:\t" << it->first << endl;
				text << "Type:\t\t";
				
				switch( it->second->getType() )
				{
					case Json::Object:
						text << "object";
						break;
						
					case Json::Array:
						text << "array";
						break;
						
					case Json::Number:
						text << "number";
						break;
						
					case Json::String:
						text << "string";
						break;
						
					case Json::Boolean:
						text << "boolean";
						break;
						
					case Json::Null:
						text << "null";
						break;
						
					default:
						text << "unknown";
						break;
				}
				
				text << endl;
				text << "Value:\t\t" << it->second->toString() << endl;
			}
			
			return text.str();
		}
	}
}

