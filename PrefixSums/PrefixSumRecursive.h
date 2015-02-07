#pragma once
#ifndef RecursiveSum_h__
#define RecursiveSum_h__

#include "stdafx.h"
#include "OpenCL.h"
typedef ReferenceCounted< OpenCLKernel_tpl<1>> OpenCLKernelPtr;

class PrefixSumRecursive
{
public:
	PrefixSumRecursive( OpenCLKernelPtr sumKernel, OpenCLKernelPtr tmpSumsKernel, OpenCLBufferPtr inputBuffer, size_t numElements );

	void Run();

	void Clear();

private:
	OpenCLKernelPtr m_SumKernel;
	OpenCLKernelPtr m_TmpSumKernel;
	OpenCLBufferPtr m_InputBuffer;
	OpenCLBufferPtr m_OutputBuffer;
	OpenCLBufferPtr m_CacheBuffer;
	int m_NumGroups;
	int m_NumElements;
};

#endif // RecursiveSum_h__
