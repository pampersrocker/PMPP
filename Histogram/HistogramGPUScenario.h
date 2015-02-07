#pragma once
#ifndef HistogramGPUScenario_h__
#define HistogramGPUScenario_h__

#include "OpenCL.h"

class HistogramGPUScenario
{
public:
	HistogramGPUScenario( 
		ReferenceCounted< OpenCLKernel_tpl<1>> calcStatisticsKernel, 
		ReferenceCounted< OpenCLKernel_tpl<1>> reduceStatisticKernel,
		ReferenceCounted< OpenCLKernel_tpl<1>> atomicKernel,
		sf::Image image );
	~HistogramGPUScenario();


	ReferenceCounted< OpenCLKernel_tpl<1>> CalcStatisticsKernel;
	ReferenceCounted< OpenCLKernel_tpl<1>> AtomicKernel;
	ReferenceCounted< OpenCLKernel_tpl<1>> ReduceStatisticsKernel;

	sf::Image Image;

	std::string ToString() const;
private:

};

#endif // HistogramGPUScenario_h__
