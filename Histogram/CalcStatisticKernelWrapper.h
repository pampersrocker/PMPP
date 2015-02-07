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

	~CalcStatisticKernelWrapper();

private:

	sf::Image m_Image;
	OpenCLKernelPtr m_CalcStatisticKernel;
	OpenCLKernelPtr m_ReduceStatisticKernel;

	OpenCLBufferPtr m_ImageBuffer;

	std::vector< cl_uchar4 > m_ImageData;

	std::array< int, 256 > m_ResultArray;


};

#endif // CalcStaticKernelWrapper_h__
