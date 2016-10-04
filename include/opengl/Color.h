#ifndef __OPENGL_COLOR_H
#define __OPENGL_COLOR_H	1

#include <string>
using namespace std;

namespace opengl
{
	class Color
	{	
		protected:
			float red;
			float green;
			float blue;
			float alpha;
			
			void parseHexString( const string& hexstring );
			
		public:
			Color( float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f );
			Color( const string& hexstring );
			Color( const Color& color );
			virtual ~Color();
			
			void setRed( float red );
			void setGreen( float green );
			void setBlue( float blue );
			void setAlpha( float alpha );
			void setColor( const string& hexstring );
			void setColor( const Color& color );
			
			float getRed() const;
			float getGreen() const;
			float getBlue() const;
			float getAlpha() const;
			string getHexString( bool printAlpha = false ) const;
	};
}

#endif
