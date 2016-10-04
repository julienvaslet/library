#ifndef __OPENGL_MATRIX_H
#define __OPENGL_MATRIX_H	1

using namespace std;

#if defined(__MINGW32__) || defined(__MINGW64__)
#undef far
#undef near
#endif

/*
 * Note on matrix storage:
 *
 * OpenGL uses column-major matrices to operate, and this class uses this format.
 * Traditional mathematics uses row-major.
 * To get a mathematic usable matrix, simply transpose() it.
 */

namespace opengl
{
	class Matrix
	{
		public:
			static Matrix projection;
			static Matrix modelview;
			
		protected:
			float matrix[16];
			
		public:
			Matrix();
			Matrix( const float matrix[16], bool columnMajor = true );
			Matrix( const Matrix& matrix );
			virtual ~Matrix();
			
			const float * get() const;
			const float get( unsigned int line, unsigned int column ) const;
			
			void set( const float matrix[16], bool columnMajor = true );
			void set( unsigned int line, unsigned int column, float value );
		
			void transpose();
			void multiply( Matrix& matrix );

#ifndef __PI__
			void load() const;
#endif
			void show() const;

			static Matrix identity();
			static Matrix translation( float x, float y, float z );
			static Matrix scale( float factor );
			static Matrix scale( float x, float y, float z );
			static Matrix rotationX( float angle );
			static Matrix rotationY( float angle );
			static Matrix rotationZ( float angle );
			static Matrix rotation( float x, float y, float z );
		
			static Matrix ortho( float left, float right, float bottom, float top, float near, float far );
			static Matrix perspective( float fovy, float aspect, float near, float far );
			static Matrix lookAt( float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz );
			static Matrix frustum( float left, float right, float bottom, float top, float near, float far );
	};
}

#endif
