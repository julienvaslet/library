#ifndef __DATA_JSON_OBJECT
#define __DATA_JSON_OBJECT	1

#include <data/json/Json.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

namespace data
{
	namespace json
	{
		class Object : public Json
		{
			protected:
				map<string, Json *> values;
				
				Object( DOM * dom = NULL );
			
			public:
				static Object * create( DOM * dom = NULL );
				virtual ~Object();

				unsigned int length() const;
				void set( const string& key, Json * value );
				bool hasKey( const string& key ) const;
				void erase( const string& key );
				
				Json * operator[] ( const string& key );
				vector<string> keys() const;
				
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

