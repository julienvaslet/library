#ifndef __DATA_JSON_DOM
#define __DATA_JSON_DOM	1

#include <data/json/Json.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

namespace data
{
	namespace json
	{
		class Object;
	
		class DOM
		{
			protected:
				map<string, Json *> variables;
				
				void free( const string& path );
				bool pathAnalysis( const string& path, queue<string>& names, queue<int>& indices );
			
			public:
				DOM( Object * object = NULL );
				~DOM();
				
				//bool exists( const string& path ) const;
				
				bool set( const string& path, Json * value );
				bool set( const string& path, const string& value );
				bool set( const string& path, const char * value );
				bool set( const string& path, int value );
				bool set( const string& path, double value );
				bool set( const string& path, bool value );
				
				Json * get( const string& path );
				
				string toString() const;
		};
	}
}

#endif

