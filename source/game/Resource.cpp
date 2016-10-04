#include <game/Resource.h>
#include <data/image/Image.h>
#include <opengl/Texture2D.h>

using namespace data::image;
using namespace opengl;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace game
{
	map<string, Resource *> Resource::resources;
		
	Resource::Resource() : instances(0), persistent(false)
	{
	}
	
	Resource::~Resource()
	{
		#ifdef DEBUG1
		map<string, Resource *>::const_iterator it;
		
		for( it = Resource::resources.begin() ; it != Resource::resources.end() ; it++ )
		{
			if( it->second == this )
				break;
		}
		
		if( it != Resource::resources.end() )
			Logger::get() << "[Resource#" << it->first << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[Resource] Unknown resource deleted." << Logger::endl;
		#endif
	}
	
	void Resource::use()
	{
		this->instances++;
	}
	
	void Resource::free()
	{
		// Prevent from free without use
		if( this->instances > 0 )
			this->instances--;
		
		if( this->instances == 0 && !persistent )
		{
			map<string, Resource *>::const_iterator it;
		
			for( it = Resource::resources.begin() ; it != Resource::resources.end() ; it++ )
			{
				if( it->second == this )
					break;
			}
		
			if( it != Resource::resources.end() )
			{
				delete it->second;
				Resource::resources.erase( it->first );
			}
			else
				delete this;
		}
		
	}
	
	void Resource::load( const string& name, Resource * resource, bool persistent )
	{
		map<string, Resource *>::iterator it = Resource::resources.find( name );
		
		if( it != Resource::resources.end() )
			delete it->second;
		
		resource->persistent = persistent;
		Resource::resources[name] = resource;
	}
	
	void Resource::loadTexture2D( const string& name, const string& filename, bool persistent )
	{
		Image * image = Image::load( filename );
		Texture2D * texture = new Texture2D();
		texture->setData( *image );
		delete image;
		
		Resource::load( name, texture, persistent );
	}
	
	Resource * Resource::get( const string& name )
	{
		Resource * resource = NULL;
		
		map<string, Resource *>::iterator it = Resource::resources.find( name );
		
		if( it != Resource::resources.end() )
			resource = it->second;
		
		return resource;
	}
	
	void Resource::destroy()
	{
		for( map<string, Resource *>::iterator it = Resource::resources.begin() ; it != Resource::resources.end() ; it++ )
			delete it->second;
			
		Resource::resources.clear();
	}		
}
