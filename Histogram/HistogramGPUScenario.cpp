#include "stdafx.h"
#include "HistogramGPUScenario.h"

HistogramGPUScenario::HistogramGPUScenario( 
	ReferenceCounted< OpenCLKernel_tpl<1>> calcStatisticsKernel, 
	ReferenceCounted< OpenCLKernel_tpl<1>> reduceStatisticsKernel,
	sf::Image image) :
CalcStatisticsKernel(calcStatisticsKernel),
ReduceStatisticsKernel(reduceStatisticsKernel),
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
