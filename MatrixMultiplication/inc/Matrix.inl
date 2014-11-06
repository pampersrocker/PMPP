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
}

template <typename T>
Matrix_tpl<T>::Matrix_tpl( const Matrix_tpl<T>& rhs )
{
	m_SizeX = rhs.m_SizeX;
	m_SizeY = rhs.m_SizeY;
	m_Data = new T[ m_SizeX*m_SizeY ];
	memcpy( m_Data, rhs.m_Data, m_SizeX*m_SizeY*sizeof( T ) );
}

template <typename T>
Matrix_tpl<T>::Matrix_tpl() :
	m_SizeX(0),
	m_SizeY(0),
	m_Data(nullptr)
{

}

template <typename T>
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
Matrix_tpl<T> Matrix_tpl<T>::operator*( const Matrix_tpl<T>& rhs ) const
{
	if( m_SizeX == rhs.m_SizeY )
	{
		Matrix_tpl<T> result( rhs.m_SizeX, m_SizeY );
		for( size_t y = 0; y < m_SizeY; y++ )
		{
			for( size_t x = 0; x < rhs.m_SizeX; ++x )
			{
				T tmp = 0;
				for( size_t k = 0; k < m_SizeX; k++ )
				{
					tmp += m_Data[ k + m_SizeX*y ] * rhs.m_Data[ k*rhs.m_SizeX + x ];
				}
				result.m_Data[ y*result.m_SizeX + x ] = tmp;
			}
		}
		return result;
	}
	else
	{
		return Matrix_tpl<T>();
	}
}

template <typename T>
bool Matrix_tpl<T>::operator==( const Matrix_tpl<T>& rhs ) const
{
	if( m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY )
	{
		for( size_t y = 0; y < m_SizeY; y++ )
		{
			for( size_t x = 0; x < m_SizeX; x++ )
			{
				if( m_Data[y*m_SizeX+x] != rhs.m_Data[y*m_SizeX+x] )
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
bool Matrix_tpl<T>::operator!=( const Matrix_tpl<T>& rhs ) const
{
	if( m_SizeX == rhs.m_SizeX && m_SizeY == rhs.m_SizeY )
	{
		for( size_t y = 0; y < m_SizeY; y++ )
		{
			for( size_t x = 0; x < m_SizeX; x++ )
			{
				if( m_Data[ y*m_SizeX + x ] != rhs.m_Data[ y*m_SizeX + x ] )
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
