#include <opengl/Color.h>

#include <sstream>
#include <cstdlib>
#include <iostream>
using namespace std;

namespace opengl
{
	Color::Color( float red, float green, float blue, float alpha ) : red(red), green(green), blue(blue), alpha(alpha)
	{
	}
	
	Color::Color( const Color& color ) : red(color.red), green(color.green), blue(color.blue), alpha(color.alpha)
	{
	}
	
	Color::Color( const string& hexstring ) : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f)
	{
		this->parseHexString( hexstring );
	}
	
	Color::~Color()
	{
	}
	
	void Color::parseHexString( const string& hexstring )
	{		
		if( hexstring.length() >= 6 )
		{
			this->red = static_cast<float>( strtol( hexstring.substr( 0, 2 ).c_str(), 0, 16 ) ) / 255.0f;
			this->green = static_cast<float>( strtol( hexstring.substr( 2, 2 ).c_str(), 0, 16 ) ) / 255.0f;
			this->blue = static_cast<float>( strtol( hexstring.substr( 4, 2 ).c_str(), 0, 16 ) ) / 255.0f;
			
			if( hexstring.length() >= 8 )
				this->alpha = static_cast<float>( strtol( hexstring.substr( 6, 2 ).c_str(), 0, 16 ) ) / 255.0f;
		}
	}
	
	void Color::setRed( float red )
	{
		this->red = red;
	}
	
	void Color::setGreen( float green )
	{
		this->green = green;
	}
	
	void Color::setBlue( float blue )
	{
		this->blue = blue;
	}
	
	void Color::setAlpha( float alpha )
	{
		this->alpha = alpha;
	}
	
	void Color::setColor( const string& hexstring )
	{
		this->parseHexString( hexstring );
	}
	
	void Color::setColor( const Color& color )
	{
		this->red = color.red;
		this->green = color.green;
		this->blue = color.blue;
		this->alpha = color.alpha;
	}
	
	float Color::getRed() const
	{
		return this->red;
	}
	
	float Color::getGreen() const
	{
		return this->green;
	}

	float Color::getBlue() const
	{
		return this->blue;
	}
	
	float Color::getAlpha() const
	{
		return this->alpha;
	}
	
	string Color::getHexString( bool printAlpha ) const
	{
		stringstream ss;
		ss.setf( std::ios::hex, std::ios::basefield );
		ss << static_cast<int>( this->red * 255.0f ) << static_cast<int>( this->green * 255.0f ) << static_cast<int>( this->blue * 255.0f );
		
		if( printAlpha )
			ss << static_cast<int>( this->alpha );
		
		return ss.str();
	}
}
