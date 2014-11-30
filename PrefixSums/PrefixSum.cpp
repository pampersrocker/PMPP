#include "stdafx.h"
#include "PrefixSum.h"

PrefixSum::PrefixSum( const std::vector< int >& data ) :
	m_Data(data)
{

}

PrefixSum::~PrefixSum()
{

}

void PrefixSum::CalculateResult( std::vector< int >& result ) const
{
	result[ 0 ] = 0;
	for( size_t i = 1; i < m_Data.size(); i++ )
	{
		result[ i ] = result[ i - 1 ] + m_Data[ i ];
	}
}
