#include <data/json/Object.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		Object::Object( DOM * dom ) : Json( dom )
		{
			#ifdef DEBUG0
			cout << "[Json][Object#" << this->uid << "] Initialized empty object" << endl;
			#endif
		}
		
		Object * Object::create( DOM * dom )
		{
			return new Object( dom );
		}
		
		Object::~Object()
		{
			for( map<string, Json *>::iterator it = this->values.begin() ; it != this->values.end() ; it++ )
			{
				if( ( it->second->getType() != Json::Object && it->second->getType() != Json::Array && it->second->getDOM() == this->getDOM() ) || it->second->getDOM() == NULL )
					delete it->second;
			}
			
			#ifdef DEBUG0
			cout << "[Json][Object#" << this->uid << "] Deleted." << endl;
			#endif
		}
		
		unsigned int Object::length() const
		{
			return this->values.size();
		}
		
		void Object::set( const string& key, Json * value )
		{
			if( ( value->getType() != Json::Object && value->getType() != Json::Array )
			 || ( this->dom != NULL && value->getDOM() != NULL && value->getDOM() != this->dom ) )
				value = value->clone();
				
			if( this->dom != NULL )
			{
				if( value->getDOM() != this->dom )
					value->setDOM( this->dom );
			}
			
			this->values[ key ] = value;
		}
		
		bool Object::hasKey( const string& key ) const
		{
			return this->values.find( key ) != this->values.end();
		}
		
		vector<string> Object::keys() const
		{
			vector<string> k;
			
			for( map<string, Json *>::const_iterator it = this->values.begin() ; it != this->values.end() ; it++ )
				k.push_back( it->first );
			
			return k;
		}
		
		Json * Object::operator[] ( const string& key )
		{
			if( this->hasKey( key ) )
				return this->values[ key ];
			else
				return NULL;
		}
		
		void Object::erase( const string& key )
		{
			map<string, Json *>::iterator it = this->values.find( key );
			
			if( it != this->values.end() )
				this->values.erase( it );
		}
		
		Json::Type Object::getType() const
		{
			return Json::Object;
		}
		
		string Object::get() const
		{
			return "[Object]";
		}
		
		string Object::toString() const
		{
			ostringstream out;
			out << "{";
			
			bool first = true;
			for( map<string, Json *>::const_iterator it = this->values.begin() ; it != this->values.end() ; it++ )
			{
				if( first )
					first = false;
				else
					out << ",";

				out << "\"" << it->first << "\":" << it->second->toString();
			}
			
			out << "}";
			return out.str();
		}
		
		Json * Object::clone() const
		{
			return NULL;
		}
	}
}

