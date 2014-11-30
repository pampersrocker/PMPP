#pragma once
#ifndef BenchmarkPrefixSumScenario_h__
#define BenchmarkPrefixSumScenario_h__

#include <string>
#include <vector>

#include <random>
#include <functional>
#include <ctime>
class PrefixSumScenario
{
public:

	PrefixSumScenario( size_t size, size_t selectedDevice, size_t selectedPlatform );
	~PrefixSumScenario();

	std::vector< int > data;
	std::vector< int > result;
	std::vector< int > expected;


	size_t selectedDevice; 
	size_t selectedPlatform;

	std::string ToString() const;

private:

};

#endif // BenchmarkPrefixSumScenario_h__
