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
		//PrefixSumScenario( 256, selectedDeviceIdx, selectedPlatformIdx ),
		PrefixSumScenario( 512, kernel,arraySum),
		PrefixSumScenario( 1024, kernel, arraySum),
		PrefixSumScenario( 2048,kernel, arraySum),
		PrefixSumScenario( 4096,kernel, arraySum),
		PrefixSumScenario( 8192,kernel, arraySum),
		//PrefixSumScenario( 16384,kernel, arraySum),
		//PrefixSumScenario( 32768,kernel, arraySum),
		//PrefixSumScenario( 65536,kernel, arraySum),
		//PrefixSumScenario( 131072, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 2621440, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 10000000, selectedDeviceIdx, selectedPlatformIdx )
	
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

		benchmarker.Iterations( 1 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();

		clManager.Release();

		kernel->Release();
	}

}


