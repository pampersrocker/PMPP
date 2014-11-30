#pragma once
#include <stdint.h>
#include <math.h>
#include <limits>
#include <algorithm>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

inline
bool fcmp( float a, float b, float epsilon = 1e-4f )
{
	return fabs( a - b ) < (epsilon * std::max( fabs( a ), fabs( b ) ));
}

template <typename T>
class Matrix_tpl
{
public:
	Matrix_tpl();
	Matrix_tpl( uint32_t sizeX, uint32_t sizeY );
	Matrix_tpl( Matrix_tpl<T>&& rhs );
	Matrix_tpl( const Matrix_tpl<T>& rhs );

	Matrix_tpl<T>& operator = ( const Matrix_tpl<T>& rhs );
	~Matrix_tpl();

	T* Data() const;

	Matrix_tpl<T> operator * ( const Matrix_tpl<T>& rhs ) const;

	static void MatMult( const Matrix_tpl<T>& mat1, const Matrix_tpl<T>& mat2, Matrix_tpl<T>& result );


	
	/**
	\brief Access to y index first
	
	\param idx The index in y direction
	
	\return A pointer to T starting at the specified row
	*/
	T* operator[] ( uint32_t idx ) const;

	bool operator == ( const Matrix_tpl<T>& rhs ) const;
	bool operator != ( const Matrix_tpl<T>& rhs ) const;

	uint32_t GetErrorIdx( const Matrix_tpl<T>& rhs ) const;

	std::string ToString() const;

	uint32_t SizeX() const;
	uint32_t SizeY() const;

private:

	uint32_t m_SizeX;
	uint32_t m_SizeY;
	
	T* m_Data;

};




#include "Matrix.inl"
