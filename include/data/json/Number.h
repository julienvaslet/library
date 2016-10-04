#ifndef __DATA_JSON_NUMBER
#define __DATA_JSON_NUMBER	1

#include <data/json/Value.h>
#include <string>
using namespace std;

namespace data
{
	namespace json
	{
		class Number : public Value
		{
			protected:
				double value;
			
				Number( double value, DOM * dom = NULL );
				
			public:
				static Number * create( double value = 0.0, DOM * dom = NULL );
				virtual ~Number();
				
				double getValue() const;
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

