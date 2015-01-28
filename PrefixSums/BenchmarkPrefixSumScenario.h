#pragma once
#ifndef BenchmarkPrefixSumScenario_h__
#define BenchmarkPrefixSumScenario_h__

#include <string>
#include <vector>

#include <random>
#include <functional>
#include <ctime>
#include "OpenCL.h"
class PrefixSumScenario
{
public:

	PrefixSumScenario( size_t size, ReferenceCounted< OpenCLKernel_tpl< 1 >> kernel, ReferenceCounted< OpenCLKernel_tpl< 1 >> arraySum);
	~PrefixSumScenario();

	std::vector< int > data;
	std::vector< int > result;
	std::vector< int > expected;


	ReferenceCounted< OpenCLKernel_tpl< 1 >> kernel;
	ReferenceCounted< OpenCLKernel_tpl< 1 >> arraySumKernel;

	std::string ToString() const;

private:

};

#endif // BenchmarkPrefixSumScenario_h__
