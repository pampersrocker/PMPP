#include "stdafx.h"
#include "HistogramGPUScenario.h"

HistogramGPUScenario::HistogramGPUScenario( 
	ReferenceCounted< OpenCLKernel_tpl<1>> calcStatisticsKernel, 
	ReferenceCounted< OpenCLKernel_tpl<1>> reduceStatisticsKernel,
	ReferenceCounted< OpenCLKernel_tpl<1>> atomicKernel,
	sf::Image image) :
CalcStatisticsKernel(calcStatisticsKernel),
AtomicKernel( atomicKernel),
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
