#pragma once
#ifndef CalcStaticKernelWrapper_h__
#define CalcStaticKernelWrapper_h__

#include "stdafx.h"
#include <array>

class CalcStatisticKernelWrapper
{
public:
	CalcStatisticKernelWrapper( OpenCLKernelPtr calcStaticKernel, OpenCLKernelPtr reduceStatisticKernel, OpenCLKernelPtr atomicKernel );

	void SetImage( sf::Image image );

	void Run();

	void ReadOutput( std::array<int, 256>& out);

	~CalcStatisticKernelWrapper();

	const std::array< int, 256 >& ResultArray() const;
	size_t NumPixelsPerThread() const;
	void NumPixelsPerThread( size_t val );
	bool UseAtomicKernel() const;
	void UseAtomicKernel( bool val );

private:

	void RunNonAtomic( int numGroups, const int workGroupSize );
	void RunAtomic( int numGroups, const int workGroupSize );
	sf::Image m_Image;
	OpenCLKernelPtr m_CalcStatisticKernel;
	OpenCLKernelPtr m_ReduceStatisticKernel;
	OpenCLKernelPtr m_AtomicKernel;

	size_t m_NumPixelsPerThread;
	OpenCLBufferPtr m_ImageBuffer;
	OpenCLBufferPtr m_ResultBuffer;

	std::vector< cl_uchar4 > m_ImageData;
	bool m_UseAtomicKernel;
};

#endif // CalcStaticKernelWrapper_h__
