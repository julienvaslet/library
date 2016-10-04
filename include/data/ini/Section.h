#ifndef __DATA_INI_SECTION_H
#define __DATA_INI_SECTION_H	1

#include <string>
#include <map>
#include <set>
using namespace std;

namespace data
{
	namespace ini
	{
		class Section
		{
			protected:
				string name;
				map<string,string> values;
	
			public:
				Section( const string& name );
				~Section();
				
				const string& getName() const;
				set<string> getKeys() const;
				
				bool hasKey( const string& key ) const;
				void setValue( const string& key, const string& value );
				const string& getValue( const string& key );
		};
	}
}

#endif

