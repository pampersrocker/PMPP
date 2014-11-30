#pragma once
#ifndef PrefixSum_h__
#define PrefixSum_h__
#include "stdafx.h"
#include <vector>

class PrefixSum
{
public:
	PrefixSum(const std::vector< int >& data);
	~PrefixSum();

	void CalculateResult( std::vector< int >& result ) const;

private:

	const std::vector< int >& m_Data;

};

#endif // PrefixSum_h__
