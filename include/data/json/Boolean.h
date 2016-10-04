#ifndef __DATA_JSON_BOOLEAN
#define __DATA_JSON_BOOLEAN	1

#include <data/json/Value.h>
#include <string>
using namespace std;

namespace data
{
	namespace json
	{
		class Boolean : public Value
		{
			protected:
				bool value;
				
				Boolean( bool value, DOM * dom = NULL );
			
			public:
				static Boolean * create( bool value = true, DOM * dom = NULL );
				virtual ~Boolean();
				
				bool getValue() const;
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

