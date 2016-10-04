#ifndef __DATA_JSON_STRING
#define __DATA_JSON_STRING	1

#include <data/json/Value.h>
#include <string>
using namespace std;

namespace data
{
	namespace json
	{
		class String : public Value
		{
			protected:
				string value;
				
				String( const string& value = "", DOM * dom = NULL );
			
			public:
				static String * create( const string& value = "", DOM * dom = NULL );
				virtual ~String();
				
				const string& getValue() const;
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

