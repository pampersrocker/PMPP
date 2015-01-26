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

private:
	OpenCLKernelPtr m_SumKernel;
	OpenCLKernelPtr m_TmpSumKernel;
	OpenCLBufferPtr m_InputBuffer;
	OpenCLBufferPtr m_OutputBuffer;
	OpenCLBufferPtr m_CacheBuffer;
};

#endif // RecursiveSum_h__
