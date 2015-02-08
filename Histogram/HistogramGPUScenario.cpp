#include "stdafx.h"
#include "HistogramGPUScenario.h"

HistogramGPUScenario::HistogramGPUScenario( 
	ReferenceCounted< OpenCLKernel_tpl<1>> calcStatisticsKernel, 
	ReferenceCounted< OpenCLKernel_tpl<1>> reduceStatisticsKernel,
	ReferenceCounted< OpenCLKernel_tpl<1>> atomicKernel,
	ReferenceCounted< OpenCLKernel_tpl<1>> atomicKernelRGB,
	sf::Image image) :
CalcStatisticsKernel(calcStatisticsKernel),
AtomicKernel( atomicKernel ),
AtomicKernelRGB( atomicKernelRGB ),
ReduceStatisticsKernel( reduceStatisticsKernel ),
Image(image)
{

}

HistogramGPUScenario::~HistogramGPUScenario()
{

}

std::string HistogramGPUScenario::ToString() const
{
	return "DefaultGPUScenario";
}
