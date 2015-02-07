#include "stdafx.h"
#include "CalcStatisticKernelWrapper.h"

#define CONCAT2( a, b ) a##b
#define CONCAT( a , b ) CONCAT2( a, b )

// Macro to simplify implementation for multiple benchmarks with different number of pixel per threads
#define IMPLEMENT_GROUPED_SCENARIO( name, pixelperThreads )	\
BPP_BEGIN_BENCHMARK( Histogram, name )						\
CalcStatisticKernelWrapper* wrapper;						\
BPP_INITIALIZE_BENCHMARK									\
{															\
	wrapper = new CalcStatisticKernelWrapper( 				\
	curScenario->CalcStatisticsKernel, 						\
	curScenario->ReduceStatisticsKernel,					\
	curScenario->AtomicKernel );							\
	wrapper->SetImage( curScenario->Image );				\
	wrapper->NumPixelsPerThread( pixelperThreads );			\
															\
}															\
BPP_BENCHMARK												\
{															\
	wrapper->Run();											\
}															\
															\
BPP_RELEASE_BENCHMARK										\
{															\
	delete wrapper;											\
}															\
															\
BPP_END_BENCHMARK

IMPLEMENT_GROUPED_SCENARIO( GPUGrouped32, 32 )
IMPLEMENT_GROUPED_SCENARIO( GPUGrouped64, 64 )
IMPLEMENT_GROUPED_SCENARIO( GPUGrouped128, 128 )
IMPLEMENT_GROUPED_SCENARIO( GPUGrouped256, 256 )
IMPLEMENT_GROUPED_SCENARIO( GPUGrouped512, 512 )
IMPLEMENT_GROUPED_SCENARIO( GPUGrouped1024, 1024 )