#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#ifndef __PI__
#include <GL/glu.h>
#endif

#include <opengl/Matrix.h>
#include <opengl/Vector.h>

namespace opengl
{
	Matrix Matrix::projection = Matrix::identity();
	Matrix Matrix::modelview = Matrix::identity();
	
	Matrix::Matrix()
	{
		for( int i = 0 ; i < 16 ; i++ )
			this->matrix[ i ] = 0.0f;
	}
	
	Matrix::Matrix( const float matrix[16], bool columnMajor )
	{
		this->set( matrix, columnMajor );
	}
	
	Matrix::Matrix( const Matrix& matrix )
	{
		this->set( matrix.get() );
	}
	
	Matrix::~Matrix()
	{
	}
	
	const float * Matrix::get() const
	{
		return this->matrix;
	}
	
	const float Matrix::get( unsigned int line, unsigned int column ) const
	{
		return this->matrix[ (column - 1) * 4 + (line - 1) ];
	}
	
	void Matrix::set( const float matrix[16], bool columnMajor )
	{
		for( int i = 0 ; i < 16 ; i++ )
			this->matrix[i] = matrix[i];
	
		if( !columnMajor )
			this->transpose();
	}
	
	void Matrix::set( unsigned int line, unsigned int column, float value )
	{
		this->matrix[ (column - 1) * 4 + (line - 1) ] = value;
	}

	void Matrix::transpose()
	{
		float matrix[16];
	
		for( int i = 0 ; i < 16 ; i++ )
			matrix[i] = this->matrix[i];
		
		for( int i = 0 ; i < 4 ; i++ )
			for( int j = 0 ; j < 4 ; j++ )
				this->matrix[i * 4 + j] = matrix[i + j * 4];
	}
	
	void Matrix::multiply( Matrix& matrix )
	{
		Matrix result;
	
		for( int i = 1 ; i <= 4 ; i++ )
		{
			for( int j = 1 ; j <= 4 ; j++ )
			{
				float value = 0.0f;
			
				for( int k = 1 ; k <= 4 ; k++ )
					value += this->get( i, k ) * matrix.get( k, j );
			
				result.set( i, j, value );
			}
		}
	
		this->set( result.get() );
	}

#ifndef __PI__
	void Matrix::load() const
	{
		glLoadMatrixf( this->matrix );
	}
#endif
	
	void Matrix::show() const
	{
		for( int i = 0 ; i < 4 ; i++ )
		{
			std::cout << "[";
		
			for( int j = 0 ; j < 4 ; j++ )
				std::cout << "\t" << this->matrix[i + j * 4];
		
			std::cout << "\t]" << std::endl;
		}
	}

	Matrix Matrix::identity()
	{	
		float matrix[16] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::translation( float x, float y, float z )
	{
		float matrix[16] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::scale( float factor )
	{
		float matrix[16] =
		{
			factor, 0.0f, 0.0f, 0.0f,
			0.0f, factor, 0.0f, 0.0f,
			0.0f, 0.0f, factor, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::scale( float x, float y, float z )
	{
		float matrix[16] =
		{
			x, 0.0f, 0.0f, 0.0f,
			0.0f, y, 0.0f, 0.0f,
			0.0f, 0.0f, z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::rotationX( float angle )
	{
		float rad = angle / 180.0f * M_PI;
	
		float matrix[16] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, static_cast<float>( cos(rad) ), static_cast<float>( sin(rad) ), 0.0f,
			0.0f, static_cast<float>( -sin(rad) ), static_cast<float>( cos(rad) ), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::rotationY( float angle )
	{
		float rad = angle / 180.0f * M_PI;
		float matrix[16] =
		{
			static_cast<float>( cos(rad) ), 0.0f, static_cast<float>( -sin(rad) ), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			static_cast<float>( sin(rad) ), 0.0f, static_cast<float>( cos(rad) ), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::rotationZ( float angle )
	{
		float rad = angle / 180.0f * M_PI;
		float matrix[16] =
		{
			static_cast<float>( cos(rad) ), static_cast<float>( sin(rad) ), 0.0f, 0.0f,
			static_cast<float>( -sin(rad) ), static_cast<float>( cos(rad) ), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::rotation( float x, float y, float z )
	{
		Matrix rotx = Matrix::rotationX( x );
		Matrix roty = Matrix::rotationY( y );
		Matrix rotz = Matrix::rotationZ( z );
		rotx.multiply( roty );
		rotx.multiply( rotz );
	
		return rotx;
	}
	
	Matrix Matrix::ortho( float left, float right, float bottom, float top, float near, float far )
	{
		float matrix[16] =
		{
			2.0f / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (far - near), 0.0f,
			-1 * ((right + left) / (right - left)), -1 * ((top + bottom) / (top - bottom)), -1 * ((far + near) / (far - near)), 1.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::perspective( float fovy, float aspect, float near, float far )
	{
		float f = 1 / tan(fovy / 360.0f * M_PI);
		float matrix[16] =
		{
			f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, f, 0.0f, 0.0f,
			0.0f, 0.0f, (far + near) / (near - far), -1.0f,
			0.0f, 0.0f, (2 * far * near) / (near - far), 0.0f
		};
	
		return Matrix( matrix );
	}
	
	Matrix Matrix::lookAt( float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz )
	{
		Vector f = Vector( cx - ex, cy - ey, cz - ez );
		Vector u = Vector( upx, upy, upz );
	
		f.normalize();
		u.normalize();
	
		Vector s = f * u;
		s.normalize();
	
		u = s * f;
		f *= -1;
	
		float matrix[16] =
		{
			s.getX(), u.getX(), f.getX(), 0.0f,
			s.getY(), u.getY(), f.getY(), 0.0f,
			s.getZ(), u.getZ(), f.getZ(), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	
		Matrix m = Matrix( matrix );
		Matrix translation = Matrix::translation( -ex, -ey, -ez );
		m.multiply( translation );

		return m;
	}
	
	Matrix Matrix::frustum( float left, float right, float bottom, float top, float near, float far )
	{
		float matrix[16] =
		{
			(2.0f * near) / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, (2.0f * near) / (top - bottom), 0.0f, 0.0f,
			(right + left) / (right - left), (top + bottom) / (top - bottom), -1 * (far + near) / (far - near), -1.0f,
			0.0f, 0.0f, (-2.0f * far * near) / (far - near), 0.0f
		};
	
		return Matrix( matrix );
	}	
}
