#include <data/json/Json.h>

#include <iostream>
using namespace std;

namespace data
{
	namespace json
	{
		unsigned int Json::currentUId = 1;
		
		Json::Json( DOM * dom ) : uid(0), dom(dom)
		{
			this->uid = currentUId++;
		}
		
		Json::~Json()
		{
		}
		
		void Json::setDOM( DOM * dom )
		{
			this->dom = dom;
		}
		
		DOM * Json::getDOM()
		{
			return this->dom;
		}
		
		json::Object * Json::asObject()
		{
			json::Object * o = NULL;
			
			if( this->getType() == Json::Object )
				o = static_cast<json::Object *>( this );
			
			return o;
		}
		
		json::Array * Json::asArray()
		{
			json::Array * a = NULL;
			
			if( this->getType() == Json::Array )
				a = static_cast<json::Array *>( this );
			
			return a;
		}
		
		json::Boolean * Json::asBoolean()
		{
			json::Boolean * b = NULL;
			
			if( this->getType() == Json::Boolean )
				b = static_cast<json::Boolean *>( this );
			
			return b;
		}
		
		json::Number * Json::asNumber()
		{
			json::Number * n = NULL;
			
			if( this->getType() == Json::Number )
				n = static_cast<json::Number *>( this );
			
			return n;
		}
		
		json::String * Json::asString()
		{
			json::String * s = NULL;
			
			if( this->getType() == Json::String )
				s = static_cast<json::String *>( this );
			
			return s;
		}
	}
}

