#ifndef __DATA_JSON_JSON
#define __DATA_JSON_JSON	1

#include <string>
using namespace std;

namespace data
{
	namespace json
	{	
		class DOM;
		class Object;
		class Array;
		class String;
		class Number;
		class Boolean;
		
		class Json
		{
			private:
				static unsigned int currentUId;
				
			protected:
				unsigned int uid;
				DOM * dom;
				
				Json( DOM * dom = NULL );
			
			public:				
				enum Type
				{
					Object,
					Array,
					Number,
					String,
					Boolean,
					Null
				};
				
				virtual ~Json();
				virtual Json::Type getType() const = 0;
				virtual string get() const = 0;
				virtual string toString() const = 0;
				virtual Json * clone() const = 0;
				
				void setDOM( DOM * dom );
				DOM * getDOM();
				
				json::Object * asObject();
				json::Array * asArray();
				json::Boolean * asBoolean();
				json::Number * asNumber();
				json::String * asString();
		};
	}
}

#include <data/json/DOM.h>
#include <data/json/Object.h>
#include <data/json/Array.h>
#include <data/json/Value.h>

#endif
