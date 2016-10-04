#include <data/json/Array.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		Array::Array( DOM * dom ) : Json( dom )
		{
			#ifdef DEBUG0
			cout << "[Json][Array#" << this->uid << "] Initialized empty array" << endl;
			#endif
		}
		
		Array * Array::create( DOM * dom )
		{
			return new Array( dom );
		}
		
		Array::~Array()
		{
			for( deque<Json *>::iterator it = this->values.begin() ; it != this->values.end() ; it++ )
			{
				if( ( (*it)->getType() != Json::Object && (*it)->getType() != Json::Array && (*it)->getDOM() == this->getDOM() ) || (*it)->getDOM() == NULL )
					delete *it;
			}
			
			#ifdef DEBUG0
			cout << "[Json][Array#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		unsigned int Array::length() const
		{
			return this->values.size();
		}
		
		Json * Array::get( unsigned int index )
		{
			return this->values[ index ];
		}
		
		void Array::set( unsigned int index, Json * value )
		{
			if( ( value->getType() != Json::Object && value->getType() != Json::Array )
			 || ( this->dom != NULL && value->getDOM() != NULL && value->getDOM() != this->dom ) )
				value = value->clone();
				
			if( this->dom != NULL )
			{
				if( value->getDOM() != this->dom )
					value->setDOM( this->dom );
			}

			this->values[ index ] = value;
		}
		
		void Array::push( Json * value )
		{
			if( ( value->getType() != Json::Object && value->getType() != Json::Array )
			 || ( this->dom != NULL && value->getDOM() != NULL && value->getDOM() != this->dom ) )
				value = value->clone();
				
			if( this->dom != NULL )
			{
				if( value->getDOM() != this->dom )
					value->setDOM( this->dom );
			}
			
			this->values.push_back( value );
		}
		
		Json * Array::pop()
		{
			Json * value = this->values.back();
			this->values.pop_back();
			
			return value;
		}
		
		void Array::unshift( Json * value )
		{
			if( ( value->getType() != Json::Object && value->getType() != Json::Array )
			 || ( this->dom != NULL && value->getDOM() != NULL && value->getDOM() != this->dom ) )
				value = value->clone();
				
			if( this->dom != NULL )
			{
				if( value->getDOM() != this->dom )
					value->setDOM( this->dom );
			}
			
			this->values.push_front( value );
		}
		
		Json * Array::shift()
		{
			Json * value = this->values.front();
			this->values.pop_front();
			
			return value;
		}
		
		Json::Type Array::getType() const
		{
			return Json::Array;
		}
		
		string Array::get() const
		{
			return "[Array]";
		}
		
		string Array::toString() const
		{
			ostringstream out;
			out << "[";
			
			bool first = true;
			for( deque<Json *>::const_iterator it = this->values.begin() ; it != this->values.end() ; it++ )
			{
				if( first )
					first = false;
				else
					out << ",";

				out << (*it)->toString();
			}
			
			out << "]";
			return out.str();
		}
		
		Json * Array::clone() const
		{
			return NULL;
		}
	}
}

