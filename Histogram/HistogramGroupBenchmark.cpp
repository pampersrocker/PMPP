#include "stdafx.h"
#include "CalcStatisticKernelWrapper.h"

// Macro to simplify implementation for multiple benchmarks with different number of pixel per threads
#define IMPLEMENT_GROUPED_BENCHMARK( name, useAtomic , pixelperThreads )	\
BPP_BEGIN_BENCHMARK( Histogram, name )									\
CalcStatisticKernelWrapper* wrapper;									\
BPP_INITIALIZE_BENCHMARK												\
{																		\
	wrapper = new CalcStatisticKernelWrapper( 							\
	curScenario->CalcStatisticsKernel, 									\
	curScenario->ReduceStatisticsKernel,								\
	curScenario->AtomicKernel,											\
	curScenario->AtomicKernelRGB );										\
	wrapper->SetImage( curScenario->Image );							\
	wrapper->NumPixelsPerThread( pixelperThreads );						\
	wrapper->UseAtomicKernel( useAtomic );						\
																		\
}																		\
BPP_BENCHMARK															\
{																		\
	wrapper->Run();														\
}																		\
																		\
BPP_RELEASE_BENCHMARK													\
{																		\
	delete wrapper;														\
}																		\
																		\
BPP_END_BENCHMARK

IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_8, false, 8 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_16, false, 16 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_32, false, 32 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_64, false, 64 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_128, false, 128 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_256, false, 256 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_512, false, 512 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_1024, false, 1024 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_2048, false, 2048 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_4096, false, 4096 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Grouped_8192, false, 8192 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_8, true, 8 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_16, true, 16 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_32, true, 32 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_64, true, 64 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_128, true, 128 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_256, true, 256 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_512, true, 512 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_1024, true, 1024 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_2048, true, 2048 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_4096, true, 4096 )
IMPLEMENT_GROUPED_BENCHMARK( GPU_Atomic_8192, true, 8192 )
