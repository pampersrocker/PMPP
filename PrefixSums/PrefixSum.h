#pragma once
#ifndef PrefixSum_h__
#define PrefixSum_h__
#include "stdafx.h"
#include <vector>
#include "PrefixSumRecursive.h"

typedef ReferenceCounted< OpenCLKernel_tpl<1U> > OpenCLKernelPtr;

class PrefixSum
{
public:
	PrefixSum(const std::vector< int >& data);
	~PrefixSum();

	void CalculateResult( std::vector< int >& result ) const;

	static bool CheckResult( const std::vector< int >* result, const std::vector< int >* expected );

	void InitOpenCL( OpenCLKernelPtr sumKernel, OpenCLKernelPtr arraySum );
	void RunOpenCL();
	void ReleaseOpenCL( const std::vector<int>& expected, std::vector<int>* result );

private:

	OpenCLKernelPtr kernel;

	OpenCLBufferPtr m_DataBuffer;

	PrefixSumRecursive* recursiveWrapper;

	std::vector< int > m_Data;
	size_t originalSize;

	int numGroups;
};

#endif // PrefixSum_h__
