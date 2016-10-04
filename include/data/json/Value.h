#ifndef __DATA_JSON_VALUE
#define __DATA_JSON_VALUE	1

#include <data/json/Json.h>
#include <string>
using namespace std;

namespace data
{
	namespace json
	{
		class Value : public Json
		{			
			public:
				Value( DOM * dom = NULL );
				virtual ~Value();
				virtual Json::Type getType() const = 0;
				virtual string get() const = 0;
				virtual string toString() const = 0;
				virtual Json * clone() const = 0;
		};
	}
}

#include <data/json/String.h>
#include <data/json/Number.h>
#include <data/json/Boolean.h>
#include <data/json/Null.h>

#endif

