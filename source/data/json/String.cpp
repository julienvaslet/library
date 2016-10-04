#include <data/json/String.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		String::String( const string& value, DOM * dom ) : Value( dom ), value(value)
		{
			#ifdef DEBUG0
			cout << "[Json][String#" << this->uid << "] Initialized with value: " << this->value << endl;
			#endif
		}
		
		String * String::create( const string& value, DOM * dom )
		{
			return new String( value, dom );
		}
		
		String::~String()
		{
			#ifdef DEBUG0
			cout << "[Json][String#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		const string& String::getValue() const
		{
			return this->value;
		}
		
		Json::Type String::getType() const
		{
			return Json::String;
		}
		
		string String::get() const
		{
			return this->value;
		}
		
		string String::toString() const
		{
			ostringstream out;
			out << "\"" << this->value << "\"";
			return out.str();
		}
		
		Json * String::clone() const
		{
			return String::create( this->value );
		}
	}
}

