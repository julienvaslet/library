#ifndef __DATA_INI_CONFIGURATION_H
#define __DATA_INI_CONFIGURATION_H	1

#include <data/ini/Section.h>

#include <string>
#include <map>
#include <set>
using namespace std;

namespace data
{
	namespace ini
	{
		class Configuration
		{
			protected:
				map<string, Section *> sections;
	
			public:
				Configuration();
				~Configuration();
				
				bool hasSection( const string& name ) const;
				void addSection( Section * section );
				void deleteSection( const string& name );
				set<string> getSections() const;
				
				bool hasKey( const string& key );
				bool hasKey( const string& key, const string& section );
				const string& getValue( const string& key );
				const string& getValue( const string& key, const string& section );
		};
	}
}

#endif

