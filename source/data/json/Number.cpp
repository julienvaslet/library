#include <data/json/Number.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		Number::Number( double value, DOM * dom ) : Value( dom ), value(value)
		{
			#ifdef DEBUG0
			cout << "[Json][Number#" << this->uid << "] Initialized with value: " << this->value << endl;
			#endif
		}
		
		Number * Number::create( double value, DOM * dom )
		{
			return new Number( value, dom );
		}
		
		Number::~Number()
		{
			#ifdef DEBUG0
			cout << "[Json][Number#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		double Number::getValue() const
		{
			return this->value;
		}
		
		Json::Type Number::getType() const
		{
			return Json::Number;
		}
		
		string Number::get() const
		{
			ostringstream out;
			out.precision( 10 );
			out << this->value;
			return out.str();
		}
		
		string Number::toString() const
		{
			ostringstream out;
			out.precision( 10 );
			out << this->value;
			return out.str();
		}
		
		Json * Number::clone() const
		{
			return Number::create( this->value );
		}
	}
}

