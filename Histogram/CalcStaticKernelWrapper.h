#pragma once
#ifndef CalcStaticKernelWrapper_h__
#define CalcStaticKernelWrapper_h__

#include "stdafx.h"
#include <array>

class CalcStaticKernelWrapper
{
public:
	CalcStaticKernelWrapper( OpenCLKernelPtr calcStaticKernel, OpenCLKernelPtr reduceStatisticKernel );

	void SetImage( sf::Image image );

	void Run();

	~CalcStaticKernelWrapper();

private:

	sf::Image m_Image;
	OpenCLKernelPtr m_CalcStaticKernel;
	OpenCLKernelPtr m_ReduceStaticKernel;

	std::vector< cl_uchar4 > m_ImageData;

	std::array< int, 256 > m_ResultArray;


};

#endif // CalcStaticKernelWrapper_h__
