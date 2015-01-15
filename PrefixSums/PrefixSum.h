#pragma once
#ifndef PrefixSum_h__
#define PrefixSum_h__
#include "stdafx.h"
#include <vector>

typedef ReferenceCounted< OpenCLKernel_tpl<1> > OpenCLKernel;

class PrefixSum
{
public:
	PrefixSum(const std::vector< int >& data);
	~PrefixSum();

	void CalculateResult( std::vector< int >& result ) const;

	static bool CheckResult( const std::vector< int >* result, const std::vector< int >* expected );

	void InitOpenCL( OpenCLKernel device );
	void RunOpenCL();
	void ReleaseOpenCL( const std::vector<int>& expected, std::vector<int>* result );

private:

	OpenCLKernel kernel;

	OpenCLKernelArgument m_ResultArgument;

	const std::vector< int >& m_Data;

};

#endif // PrefixSum_h__
