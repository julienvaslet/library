#include <data/parser/IniParser.h>
#include <algorithm>
#include <sstream>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace std;

namespace data
{
	namespace parser
	{
		IniParser::IniParser( const string& content )
		{
			// Add a new line as first symbol do the trick to
			// make this parser working
			this->symbols.push_back( "\n" );
			
			string charSymbols = ";=[]\n";
			this->readSymbols( content, charSymbols.c_str(), " \t\r" );
			
			#ifdef DEBUG0
			Logger::get() << "[IniParser] Loaded (" << this->symbols.size() << " symbols)." << Logger::endl;
			#endif
		}
		
		IniParser::~IniParser()
		{
		}

		ini::Configuration * IniParser::parse()
		{
			this->initPointer();
			this->configuration = new ini::Configuration();
			this->currentSection = NULL;

			while( !this->eop() )
			{
				if( this->emptyline() ) {}
				else if( this->comment() ) {}
				else if( this->section() ) {}
				else if( this->assignation() ) {}
				else
				{
					#ifdef DEBUG0
					Logger::get() << "[IniParser] Warning: ignoring symbol \"" << this->read() << "\"." << Logger::endl;
					#endif
					
					this->nextSymbol();
				}
			}

			return this->configuration;
		}
		
		bool IniParser::section()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->findSymbol( "\n" ) )
			{
				this->nextSymbol();
				
				if( this->findSymbol( "[" ) )
				{
					this->nextSymbol();
					stringstream name;
	
					while( !this->findSymbol( "]" ) )
					{
						name << this->read();
						this->nextSymbol();
					}
			
					this->nextSymbol();
					string nameStr = name.str();
					this->currentSection = new ini::Section( nameStr );
					this->configuration->addSection( this->currentSection );
					
					bReturn = true;
				}
			}
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::comment()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->findSymbol( "\n" ) )
			{
				this->nextSymbol();
				
				if( this->findSymbol( ";" ) )
				{
					this->nextSymbol();
					
					while( !this->findSymbol( "\n" ) && !this->eop() )
					{
						// Ignoring comment
						this->nextSymbol();
					}
					
					bReturn = true;
				}
			}
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::assignation()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->findSymbol( "\n" ) )
			{
				this->nextSymbol();
				
				stringstream key;
				stringstream value;
				
				while( !this->findSymbol( "=" ) && !this->findSymbol( " " ) && !this->eop() )
				{
					key << this->read();
					this->nextSymbol();
				}
				
				this->eatSpaces();
				
				if( this->findSymbol( "=" ) )
				{
					this->nextSymbol();
					this->eatSpaces();
					
					while( !this->findSymbol( "\n" ) && !this->eop() )
					{
						value << this->read();
						this->nextSymbol();
					}
					
					bReturn = true;
					
					if( this->currentSection == NULL )
					{
						this->currentSection = new ini::Section( "" );
						this->configuration->addSection( this->currentSection );
					}
					
					string keyStr = key.str();
					string valueStr = value.str();
					this->currentSection->setValue( keyStr, valueStr );
				}
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool IniParser::emptyline()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->findSymbol( "\n" ) )
			{
				this->nextSymbol();
				this->eatSpaces();
				
				if( this->findSymbol( "\n" ) || this->eop() )
					bReturn = true;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
	}
}

