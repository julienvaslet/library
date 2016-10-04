#include <data/ini/Section.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace ini
	{
		Section::Section( const string& name ) : name(name)
		{
			#ifdef DEBUG0
			Logger::get() << "[Ini][Section#" << this->name << "] Created." << Logger::endl;
			#endif
		}
		
		Section::~Section()
		{
			#ifdef DEBUG0
			Logger::get() << "[Ini][Section#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		const string& Section::getName() const
		{
			return this->name;
		}
		
		set<string> Section::getKeys() const
		{
			set<string> keys;
			
			for( map<string, string>::const_iterator it = this->values.begin() ; it != this->values.end() ; it++ )
				keys.insert( it->first );
			
			return keys;
		}

		bool Section::hasKey( const string& key ) const
		{
			map<string,string>::const_iterator it = this->values.find( key );
			return it != this->values.end();
		}
		
		void Section::setValue( const string& key, const string& value )
		{
			this->values[key] = value;
			
			#ifdef DEBUG0
			Logger::get() << "[Ini][Section#" << this->name << "] \"" << key << "\" = \"" << value << "\"." << Logger::endl;
			#endif
		}
		
		const string& Section::getValue( const string& key )
		{
			return this->values[key];
		}
	}
}

