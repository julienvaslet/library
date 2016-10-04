#ifndef __DATA_PARSER_JSONPARSER_H
#define __DATA_PARSER_JSONPARSER_H	1

#include <data/parser/Parser.h>
#include <data/json/Json.h>

namespace data
{
	namespace parser
	{	
		class JsonParser : public Parser
		{
			protected:
				json::Json * element;
				json::Json * currentElement;
				
				bool object();
				bool array();
				bool value();
		
				bool text();
				bool number();
				bool boolean();
				bool null();
	
			public:
				JsonParser( const string& content );
				~JsonParser();
		
				json::Json * parse();
		};
	}
}

#endif

