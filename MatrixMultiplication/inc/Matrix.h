#pragma once
#include <stdint.h>

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


	
	/**
	\brief Access to y index first
	
	\param idx The index in y direction
	
	\return A pointer to T starting at the specified row
	*/
	T* operator[] ( uint32_t idx ) const;

	bool operator == ( const Matrix_tpl<T>& rhs ) const;
	bool operator != ( const Matrix_tpl<T>& rhs ) const;

	std::string ToString() const;

private:

	uint32_t m_SizeX;
	uint32_t m_SizeY;

	T* m_Data;

};


#include "Matrix.inl"
