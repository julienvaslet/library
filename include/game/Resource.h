#ifndef __GAME_RESOURCE_H
#define __GAME_RESOURCE_H	1

#include <map>
#include <string>
using namespace std;

namespace game
{
	class Resource
	{
		protected:
			static map<string, Resource *> resources;
			unsigned int instances;
			bool persistent;
		
			Resource();
			virtual ~Resource();
			
		public:
			void use();
			void free();
			
			static void load( const string& name, Resource * resource, bool persistent = false );
			static void loadTexture2D( const string& name, const string& filename, bool persistent = false );
			static Resource * get( const string& resource );
			static void destroy();
	};
}

#endif

