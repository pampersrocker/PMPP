#include "Matrix.h"
#include <sstream>
#include <string>
#include <iomanip>

template <typename T>
inline
Matrix_tpl<T>::Matrix_tpl( uint32_t sizeX, uint32_t sizeY ) :
	m_SizeX( sizeX ),
	m_SizeY( sizeY ),
	m_Data(nullptr)
{
	if( sizeX && sizeY )
	{
		m_Data = new T[ sizeX*sizeY ];
	}
}


template <typename T>
inline
Matrix_tpl<T>& Matrix_tpl<T>::operator=( const Matrix_tpl<T>& rhs )
{
	if( m_Data != nullptr )
	{
		delete [] m_Data;
	}
	m_SizeX = rhs.m_SizeX;
	m_SizeY = rhs.m_SizeY;
	m_Data = new T[ m_SizeX*m_SizeY ];
	memcpy( m_Data, rhs.m_Data, m_SizeX*m_SizeY*sizeof( T ) );
	return *this;
}

template <typename T>
inline
Matrix_tpl<T>::Matrix_tpl( const Matrix_tpl<T>& rhs )
{
	m_SizeX = rhs.m_SizeX;
	m_SizeY = rhs.m_SizeY;
	m_Data = new T[ m_SizeX*m_SizeY ];
	memcpy( m_Data, rhs.m_Data, m_SizeX*m_SizeY*sizeof( T ) );
}

template <typename T>
inline
Matrix_tpl<T>::Matrix_tpl() :
	m_SizeX(0),
	m_SizeY(0),
	m_Data(nullptr)
{

}

template <typename T>
inline
Matrix_tpl<T>::Matrix_tpl( Matrix_tpl<T>&& rhs )
{
	m_Data = rhs.m_Data;
	m_SizeX = rhs.m_SizeX;
	m_SizeY = rhs.m_SizeY;
	rhs.m_Data = nullptr;
	rhs.m_SizeX = 0;
	rhs.m_SizeY = 0;
}


template <typename T>
inline
T* Matrix_tpl<T>::operator[]( uint32_t idx ) const
{
	return m_Data + idx * m_SizeX;
}

template <typename T>
inline
Matrix_tpl<T>::~Matrix_tpl()
{
	delete[] m_Data;
	m_Data = nullptr;
}

template <typename T>
inline
T* Matrix_tpl<T>::Data() const
{
	return m_Data;
}


template <typename T>
void Matrix_tpl<T>::MatMult( const Matrix_tpl<T>& mat1, const Matrix_tpl<T>& mat2, Matrix_tpl<T>& result )
{
	for( size_t y = 0; y < mat1.m_SizeY; y++ )
	{
		for( size_t x = 0; x < mat2.m_SizeX; ++x )
		{
			T tmp = 0;
			for( size_t k = 0; k < mat1.m_SizeX; k++ )
			{
				tmp += mat1.m_Data[ k + mat1.m_SizeX*y ] * mat2.m_Data[ k*mat2.m_SizeX + x ];
			}
			result.m_Data[ y*result.m_SizeX + x ] = tmp;
		}
	}
}


template <typename T>
inline
Matrix_tpl<T> Matrix_tpl<T>::operator*( const Matrix_tpl<T>& rhs ) const
{
	if( mat1.m_SizeX == mat2.m_SizeY )
	{
		Matrix_tpl<T> result( m_SizeX, rhs.m_SizeY );
		MatMult( *this, rhs, result );
		return result;
	}
	else
	{
		return Matrix_tpl<T>();
	}
}

template <typename T>
inline
bool Matrix_tpl<T>::operator==( const Matrix_tpl<T>& rhs ) const
{
	if( m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY )
	{
		for( size_t y = 0; y < m_SizeY; y++ )
		{
			for( size_t x = 0; x < m_SizeX; x++ )
			{
				if( !fcmp(m_Data[y*m_SizeX+x], rhs.m_Data[y*m_SizeX+x]) )
				{
					return false;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


template <typename T>
inline
uint32_t Matrix_tpl<T>::GetErrorIdx( const Matrix_tpl<T>& rhs ) const
{

	if( m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY )
	{
		for( uint32_t y = 0; y < m_SizeY; y++ )
		{
			for( uint32_t x = 0; x < m_SizeX; x++ )
			{
				if( !fcmp( m_Data[ y*m_SizeX + x ], rhs.m_Data[ y*m_SizeX + x ] ) )
				{
					return y*m_SizeX + x;
				}
			}
		}
		return 0;
	}
	else
	{
		return 0;
	}
}


template <typename T>
inline
bool Matrix_tpl<T>::operator!=( const Matrix_tpl<T>& rhs ) const
{
	if( m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY )
	{
		for( size_t y = 0; y < m_SizeY; y++ )
		{
			for( size_t x = 0; x < m_SizeX; x++ )
			{
				if( !fcmp( m_Data[ y*m_SizeX + x ], rhs.m_Data[ y*m_SizeX + x ] ) )
				{
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}


template <typename T>
inline
std::string Matrix_tpl<T>::ToString() const
{
	std::ostringstream stream;
	for( size_t y = 0; y < m_SizeY; y++ )
	{
		stream << "| ";
		for( size_t x = 0; x < m_SizeX; x++ )
		{
			stream << std::setfill( ' ' ) << std::setw( 4 ) << m_Data[ y*m_SizeX + x ] << " ";
		}
		stream << "   |" << std::endl;
	}
	return stream.str();
}

template <typename T>
inline
uint32_t Matrix_tpl<T>::SizeY() const
{
	return m_SizeY;
}

template <typename T>
inline
uint32_t Matrix_tpl<T>::SizeX() const
{
	return m_SizeX;
}
