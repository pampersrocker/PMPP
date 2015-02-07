// PrefixSums.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Logging/BPPDefaultConsoleLogger.hpp"

#include "PrefixSum.h"
#include "OpenCL.h"

int _tmain(int argc, _TCHAR* argv[])
{
	OpenCLManager clManager;
	clManager.Initialize();
	auto device = clManager.ConsoleSelectPlatformAndDevice();
	auto context = device->CreateContext();
	OpenCLKernelPtr kernel = context->CreateKernel<1>( "CL/PrefixSum.cl", "PrefixSums" );
	OpenCLKernelPtr arraySum= context->CreateKernel<1>( "CL/ArraySum.cl", "ArraySum512" );
	
	

	std::vector< PrefixSumScenario > scenarios{
		PrefixSumScenario( 256, kernel, arraySum ),
		PrefixSumScenario( 512, kernel,arraySum),
		PrefixSumScenario( 1024, kernel, arraySum),
		PrefixSumScenario( 2048,kernel, arraySum),
		PrefixSumScenario( 4096,kernel, arraySum),
		PrefixSumScenario( 2157, kernel, arraySum ),
		PrefixSumScenario( 8192, kernel, arraySum ),
		PrefixSumScenario( 10240, kernel, arraySum ),
		PrefixSumScenario( 16384, kernel, arraySum ),
		PrefixSumScenario( 32768,kernel, arraySum),
		PrefixSumScenario( 65536,kernel, arraySum),
		PrefixSumScenario( 131072, kernel, arraySum ),
		PrefixSumScenario( 2621440, kernel, arraySum ),
		PrefixSumScenario( 10000000, kernel, arraySum ),
		PrefixSumScenario( 20000000, kernel, arraySum ),
		PrefixSumScenario( 50000000, kernel, arraySum ),
		PrefixSumScenario( 100000000, kernel, arraySum ),
		// The following needs approximately 4gb of GPU ram
		//PrefixSumScenario( 1000000000, kernel, arraySum )
	
	};

	{

		using namespace bpp;
		Benchmarker& benchmarker = Benchmarker::Instance();
		DefaultConsoleLogger logger;
		benchmarker.AddLogger( &logger );

		//benchmarker.AddScenario( &scenario );

		for( auto& it : scenarios )
		{
			benchmarker.AddScenario( &it );
		}

		benchmarker.Iterations( 5 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();

		clManager.Release();

		kernel->Release();
	}

}


