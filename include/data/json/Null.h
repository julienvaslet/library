#ifndef __DATA_JSON_NULL
#define __DATA_JSON_NULL	1

#include <data/json/Value.h>
#include <string>
using namespace std;

namespace data
{
	namespace json
	{
		class Null : public Value
		{
			protected:
			
				Null( DOM * dom = NULL );
				
			public:
				static Null * create( DOM * dom = NULL );
				virtual ~Null();
				
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

