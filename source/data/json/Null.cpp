#include <data/json/Null.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		Null::Null( DOM * dom ) : Value( dom )
		{
			#ifdef DEBUG0
			cout << "[Json][Null#" << this->uid << "] Initialized null-value" << endl;
			#endif
		}
		
		Null * Null::create( DOM * dom )
		{
			return new Null( dom );
		}
		
		Null::~Null()
		{
			#ifdef DEBUG0
			cout << "[Json][Null#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		Json::Type Null::getType() const
		{
			return Json::Null;
		}
		
		string Null::get() const
		{
			return "";
		}
		
		string Null::toString() const
		{
			return "null";
		}
		
		Json * Null::clone() const
		{
			return Null::create();
		}
	}
}

