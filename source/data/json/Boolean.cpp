#include <data/json/Boolean.h>

#include <iostream>
using namespace std;

namespace data
{
	namespace json
	{
		Boolean::Boolean( bool value, DOM * dom ) : Value( dom ), value(value)
		{
			#ifdef DEBUG0
			cout << "[Json][Boolean#" << this->uid << "] Initialized with value: " << ( this->value ? "true" : "false" ) << endl;
			#endif
		}
		
		Boolean * Boolean::create( bool value, DOM * dom )
		{
			return new Boolean( value, dom );
		}
		
		Boolean::~Boolean()
		{
			#ifdef DEBUG0
			cout << "[Json][Boolean#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		bool Boolean::getValue() const
		{
			return this->value;
		}
		
		Json::Type Boolean::getType() const
		{
			return Json::Boolean;
		}
		
		string Boolean::get() const
		{
			return this->value ? "true" : "false";
		}
		
		string Boolean::toString() const
		{
			return this->value ? "true" : "false";
		}
		
		Json * Boolean::clone() const
		{
			return Boolean::create( this->value );
		}
	}
}

