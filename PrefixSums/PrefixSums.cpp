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
	OpenCLKernel kernel = context->CreateKernel<1>( "CL/PrefixSum.cl", "PrefixSums" );
	
	

	std::vector< PrefixSumScenario > scenarios{
		//PrefixSumScenario( 256, selectedDeviceIdx, selectedPlatformIdx ),
		PrefixSumScenario( 512, kernel),
		//PrefixSumScenario( 1024, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 2048, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 4096, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 8192, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 16384, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 32768, selectedDeviceIdx, selectedPlatformIdx ),
		//PrefixSumScenario( 65536, selectedDeviceIdx, selectedPlatformIdx ),
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

		benchmarker.Iterations( 5 );

		benchmarker.Run();

		benchmarker.Log();

		benchmarker.Release();
	}

}


