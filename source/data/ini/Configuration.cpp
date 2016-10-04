#include <data/ini/Configuration.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace ini
	{
		Configuration::Configuration()
		{
			#ifdef DEBUG0
			Logger::get() << "[Ini][Configuration] Initialized." << Logger::endl;
			#endif
		}
		
		Configuration::~Configuration()
		{
			for( map<string, Section *>::iterator it = this->sections.begin() ; it != this->sections.end() ; it++ )
				delete it->second;
			
			this->sections.clear();
			
			#ifdef DEBUG0
			Logger::get() << "[Ini][Configuration] Deleted." << Logger::endl;
			#endif
		}
		
		bool Configuration::hasSection( const string& name ) const
		{
			map<string, Section *>::const_iterator it = this->sections.find( name );
			return it != this->sections.end();
		}
		
		void Configuration::addSection( Section * section )
		{
			if( this->hasSection( section->getName() ) )
			{
				#ifdef DEBUG0
				Logger::get() << "[Ini][Configuration] Warning: replacing existing section \"" << section->getName() << "\"." << Logger::endl;
				#endif
				
				this->deleteSection( section->getName() );
			}
			
			this->sections[section->getName()] = section;
			
			#ifdef DEBUG0
			Logger::get() << "[Ini][Configuration] Added section \"" << section->getName() << "\"." << Logger::endl;
			#endif
		}
		
		void Configuration::deleteSection( const string& name )
		{
			map<string, Section *>::iterator it = this->sections.find( name );
			
			if( it != this->sections.end() )
			{
				delete it->second;
				this->sections.erase( it );
			}
		}
		
		set<string> Configuration::getSections() const
		{
			set<string> sections;
			
			for( map<string, Section *>::const_iterator it = this->sections.begin() ; it != this->sections.end() ; it++ )
				sections.insert( it->first );
				
			return sections;
		}
		
		bool Configuration::hasKey( const string& key )
		{
			return this->hasSection( "" ) ? this->hasKey( key, "" ) : false;
		}
		
		bool Configuration::hasKey( const string& key, const string& section )
		{
			return this->sections[section]->hasKey( key );
		}
		
		const string& Configuration::getValue( const string& key )
		{
			return this->getValue( key, "" );
		}
		
		const string& Configuration::getValue( const string& key, const string& section )
		{
			return this->sections[section]->getValue( key );
		}
	}
}
