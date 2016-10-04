#include <opengl/ui/Font.h>
#include <tools/logger/Logger.h>

using namespace tools::logger;

namespace opengl
{
	namespace ui
	{
		map<string, Font *> Font::fonts;
	
		Font::Font( const string& name )
		{
			Font::destroy( name );
			Font::fonts[name] = this;
		
			#ifdef DEBUG0
			Logger::get() << "[Font#" << name << "] Created." << Logger::endl;
			#endif
		}
	
		Font::~Font()
		{
			#ifdef DEBUG0
			map<string, Font *>::iterator it;
		
			for( it = Font::fonts.begin() ; it != Font::fonts.end() ; it++ )
			{
				if( it->second == this )
					break;
			}
		
			if( it != Font::fonts.end() )
				Logger::get() << "[Font#" << it->first << "] Deleted." << Logger::endl;
			else
				Logger::get() << "[Font] Unreferenced font deleted." << Logger::endl;
			#endif
		}
	
		Font * Font::get( const string& name )
		{
			Font * font = NULL;
		
			map<string, Font *>::iterator it = Font::fonts.find( name );
		
			if( it != Font::fonts.end() )
				font = it->second;
		
			return font;
		}
	
		void Font::destroy( const string& name )
		{
			map<string, Font *>::iterator it = Font::fonts.find( name );
		
			if( it != Font::fonts.end() )
			{
				delete it->second;
				Font::fonts.erase( it );
			}
		}
	
		void Font::destroy()
		{
			for( map<string, Font *>::iterator it = Font::fonts.begin() ; it != Font::fonts.end() ; it++ )
				delete it->second;
		
			Font::fonts.clear();
		}
	}
}

