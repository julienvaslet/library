#include <data/parser/JsonParser.h>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

namespace data
{
	namespace parser
	{
		JsonParser::JsonParser( const string& content )
		{
			vector<string> combinations;
			combinations.push_back( "\\\"" );
			combinations.push_back( "\\'" );
		
			this->readSymbols( content, "{}[]\\\"':,-.", " \t\n\r" );
			this->packSymbols( combinations );
			
			/*// Packing symbols
			for( vector<string>::iterator it = parsedSymbols.begin() ; it != parsedSymbols.end() ; it++ )
			{
				bool symbolAppended = false;
		
				// Symbols: \" & \'
				if( *it == "\\" )
				{
					if( *(++it) == "\"" )
					{
						this->symbols.push_back( "\\\"" );
						symbolAppended = true;
					}
					else if( *(++it) == "'" )
					{
						this->symbols.push_back( "\\'" );
						symbolAppended = true;
					}
					else
					 it -= 1;
				}
		
				if( !symbolAppended )
					this->symbols.push_back( *it );
			}*/
	
			#ifdef DEBUG0
			cout << "[JsonParser] Loaded (" << this->symbols.size() << " symbols)." << endl;
			#endif
		}
		
		JsonParser::~JsonParser()
		{
		}
		
		json::Json * JsonParser::parse()
		{
			this->initPointer();	
			this->currentElement = NULL;
			this->element = NULL;
			
			if( this->object() || this->array() || this->text() || this->number() || this->boolean() || this->null() )
			{
				this->element = this->currentElement;
			}
			
			return this->element;
		}
		
		bool JsonParser::object()
		{
			bool bReturn = false;
			
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->findSymbol( "{" ) )
			{
				json::Object * object = json::Object::create();
				
				do
				{
					this->nextSymbol();
					this->eatSpaces();
					
					bool singleQuote = false;
					
					if( this->findSymbol( "\"" ) || this->findSymbol( "'" ) )
					{
						singleQuote = this->findSymbol( "'" );
						this->nextSymbol();
						
						if( !this->eop() )
						{
							ostringstream name;
							
							while( !( (this->findSymbol( "\"" ) && !singleQuote) || (this->findSymbol( "'" ) && singleQuote) ) )
							{
								name << this->read();
								this->nextSymbol();
							}
							
							this->nextSymbol();
							this->eatSpaces();
							
							if( this->findSymbol( ":" ) )
							{
								this->nextSymbol();
								
								if( this->object() || this->array() || this->text() || this->number() || this->boolean() || this->null() )
								{
									object->set( name.str(), this->currentElement );
					
									if( this->currentElement->getType() != json::Json::Object && this->currentElement->getType() != json::Json::Array )
										delete this->currentElement;

									this->currentElement = NULL;
									this->eatSpaces();
								}
								else
									break;
							}
							else
								break;
						}
					}
					else
						break;
				}
				while( this->findSymbol( "," ) );
				
				if( this->findSymbol( "}" ) )
				{
					this->nextSymbol();
					bReturn = true;
					this->currentElement = object;
				}
				else
					delete object;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool JsonParser::array()
		{
			bool bReturn = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "[" ) )
			{
				this->nextSymbol();
				
				json::Array * array = json::Array::create();
				
				while( this->object() || this->array() || this->text() || this->number() || this->boolean() || this->null() )
				{
					array->push( this->currentElement );
					
					if( this->currentElement->getType() != json::Json::Object && this->currentElement->getType() != json::Json::Array )
						delete this->currentElement;
					
					this->currentElement = NULL;
					
					this->eatSpaces();
					if( this->findSymbol( "," ) )
						this->nextSymbol();
					else
						break;
				}
				
				this->eatSpaces();
				if( this->findSymbol( "]" ) )
				{
					this->nextSymbol();
					
					bReturn = true;
					this->currentElement = array;
				}
				else
					delete array;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool JsonParser::text()
		{
			bool bReturn = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			bool singleQuote = this->findSymbol( "'" );
			
			if( this->findSymbol( "\"" ) || singleQuote )
			{
				this->nextSymbol();
				
				ostringstream value;
				
				while( !this->eop() && !( (this->findSymbol( "\"" ) && !singleQuote) || (this->findSymbol( "'" ) && singleQuote) ) )
				{
					value << this->read();
					this->nextSymbol();
				}
				
				if( (this->findSymbol( "\"" ) && !singleQuote) || (this->findSymbol( "'" ) && singleQuote) )
				{
					this->nextSymbol();
					bReturn = true;
					
					this->currentElement = json::String::create( value.str() );
				}
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool JsonParser::number()
		{
			bool bReturn = false;
			double value = 0.0;
			bool negative = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "-" ) )
			{
				negative = true;
				this->nextSymbol();
			}
				
			if( !this->eop() )
			{
				bool valid = true;
				string txt = this->read();
				double decimal = 1.0;
				
				for( string::reverse_iterator rit = txt.rbegin() ; rit != txt.rend() ; rit++ )
				{
					if( *rit >= '0' && *rit <= '9' )
					{
						value += (*rit - '0') * decimal;
					}
					else
					{
						valid = false;
						break;
					}
					
					decimal *= 10.0;
				}
				
				if( valid )
				{
					this->nextSymbol();
					
					if( this->findSymbol( "." ) )
					{
						this->nextSymbol();
						
						if( !this->eop() )
						{
							double floatPart = 0.0;
							valid = true;
							txt = this->read();
							decimal = 0.1;
							
							for( string::iterator it = txt.begin() ; it != txt.end() ; it++ )
							{
								if( *it >= '0' && *it <= '9' )
								{
									floatPart += (*it - '0') * decimal;
								}
								else
								{
									valid = false;
									break;
								}
				
								decimal *= 0.1;
							}
							
							if( valid )
							{
								this->nextSymbol();
								value += floatPart;
							}
							else
								this->prevSymbol();
						}
						else
						 this->prevSymbol();
					}
					
					if( negative )
						value *= -1.0;
					
					bReturn = true;
					this->currentElement = json::Number::create( value );
				}
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool JsonParser::boolean()
		{
			bool bReturn = false;

			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "true" ) )
			{
				this->currentElement = json::Boolean::create( true );
				this->nextSymbol();
				
				bReturn = true;
			}
			else if( this->findSymbol( "false" ) )
			{
				this->currentElement = json::Boolean::create( false );
				this->nextSymbol();
				
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool JsonParser::null()
		{
			bool bReturn = false;

			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "null" ) )
			{
				this->currentElement = json::Null::create();
				this->nextSymbol();
				
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
	}
}

