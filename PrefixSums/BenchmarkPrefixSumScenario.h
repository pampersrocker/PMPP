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

	PrefixSumScenario( size_t size );
	~PrefixSumScenario();

	std::vector< int > data;
	std::vector< int > result;
	std::vector< int > expected;

	std::string ToString() const;

private:

};

#endif // BenchmarkPrefixSumScenario_h__
