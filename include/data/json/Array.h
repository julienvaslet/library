#ifndef __DATA_JSON_ARRAY
#define __DATA_JSON_ARRAY	1

#include <data/json/Json.h>
#include <data/json/Value.h>
#include <string>
#include <deque>
using namespace std;

namespace data
{
	namespace json
	{
		class Array : public Json
		{
			protected:
				deque<Json *> values;
				
				Array( DOM * dom = NULL );
			
			public:
				static Array * create( DOM * dom = NULL );
				virtual ~Array();
				
				unsigned int length() const;
				Json * get( unsigned int index );
				void set( unsigned int index, Json * value );
				void push( Json * value );
				Json * pop();
				void unshift( Json * value );
				Json * shift();
				
				virtual Json::Type getType() const;
				virtual string get() const;
				virtual string toString() const;
				virtual Json * clone() const;
		};
	}
}

#endif

