#pragma once
#ifndef CalcStaticKernelWrapper_h__
#define CalcStaticKernelWrapper_h__

#include "stdafx.h"
#include <array>

class CalcStatisticKernelWrapper
{
public:
	CalcStatisticKernelWrapper( OpenCLKernelPtr calcStaticKernel, OpenCLKernelPtr reduceStatisticKernel );

	void SetImage( sf::Image image );

	void Run();

	void ReadOutput( std::array<int, 256>& out);

	~CalcStatisticKernelWrapper();

	const std::array< int, 256 >& ResultArray() const;
	size_t NumPixelsPerThread() const;
	void NumPixelsPerThread( size_t val );
private:

	sf::Image m_Image;
	OpenCLKernelPtr m_CalcStatisticKernel;
	OpenCLKernelPtr m_ReduceStatisticKernel;

	size_t m_NumPixelsPerThread;
	OpenCLBufferPtr m_ImageBuffer;
	OpenCLBufferPtr m_ResultBuffer;

	std::vector< cl_uchar4 > m_ImageData;

};

#endif // CalcStaticKernelWrapper_h__
