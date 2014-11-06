/**********************************************************************
Copyright ©2013 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

// For clarity,error checking has been omitted.

#include <CL/cl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <bpp.hpp>
#include "Logging/BPPDefaultConsoleLogger.hpp"
#include "..\common\OpenCLProgram.h"
#include "..\common\OpenCLPlatform.h"
#include "..\common\OpenCLDevice.h"

using namespace bpp;

#define SUCCESS 0
#define FAILURE 1

using namespace std;


float summiere(float *f, int size)
{
	for ( ; size>1 ; size>>=2)
	{
		for (int i=0 ; i<size/2 ; i++)
			f[i] += f[i+size/2];
	}

	return f[0];
}
int selectedPlatformIdx = 0;
int selectedDeviceIdx = 0;

void RunHelloWorld()
{
	OpenCLProgram program;
	program.InitializeCL();
	program.SelectPlatformAndDevice( selectedPlatformIdx,selectedDeviceIdx);
	program.LoadKernel( "HelloWorld_Kernel.cl", "helloworld" );
	string hello = "Hello World";
	char tmp[ 20 ];
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ( hello.size() + 1 )*sizeof( char ), ( void * ) hello.data() );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, ( hello.size() + 1 )*sizeof( char ) );

	program.Run();

	program.ReadOutput( 1, tmp );

	tmp[ hello.size() ] = '\0';

	cout << "Input is: " << hello << std::endl;

	cout << "Output is: " << tmp << std::endl;


	program.Release();
}

void RunSumTest()
{
	OpenCLProgram program;
	program.InitializeCL();
	program.SelectPlatformAndDevice( selectedPlatformIdx,selectedDeviceIdx);
	program.LoadKernel( "Sum.cl", "sum" );
	vector<int> ints;
	int size = 512;
	ints.resize( size );
	for( size_t i = 0; i < size; i++ )
	{
		ints[ i ] = i;
	}
	int count = ints.size();

	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ints.size() * sizeof( int ), ( void * ) ints.data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( int ), &count );
	program.AddKernelArgGlobal( CL_MEM_READ_WRITE, ( ints.size() / 2 )*sizeof( int ) );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( int ) );

	program.SetFirstWorkSize( ints.size() / 2 );


	program.Run();

	int output;

	program.ReadOutput( 3, &output );

	int sum = 0;
	for( auto it : ints )
	{
		sum += it;
	}

	cout << "Expected Output is: " << sum << std::endl;
	cout << "Output is: " << output << std::endl;


	program.Release();
}



int main(int argc, char* argv[])
{

	//RunHelloWorld();

	//RunSumTest();

	OpenCLProgram program;
	program.InitializeCL();
	int i =0;
	for (auto platform : program.Platforms())
	{
		std::cout << "[ " << i++ << " ] " << platform->PlatformName() << std::endl;
	}
	cin >> selectedPlatformIdx;
	i=0;
	for (auto device : (program.Platforms()[i]->Devices()))
	{
		std::cout << "[ " << i++ << " ] " << device->cl_device_name << " (" << (device->CL_device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU") << ")" << std::endl;
	}
	cin >> selectedDeviceIdx;


	Benchmarker& benchmarker = Benchmarker::Instance();

	DefaultConsoleLogger logger;
	benchmarker.Iterations( 50 );

	benchmarker.AddLogger( &logger );

	benchmarker.Run();

	benchmarker.Log();

	benchmarker.Release();

	std::cin.get();
}

int size = 512;



BPP_BEGIN_BENCHMARK( OpenCL, GPU )
OpenCLProgram program;
BPP_INITIALIZE_BENCHMARK
{
	program = OpenCLProgram();
	program.InitializeCL();
	program.SelectPlatformAndDevice( selectedPlatformIdx,selectedDeviceIdx);
	program.LoadKernel( "Sum.cl", "sum" );
	vector<int> ints;
	ints.resize( size );
	for( size_t i = 0; i < size; i++ )
	{
		ints[ i ] = i;
	}
	int count = ints.size();

	vector<int> tmp( ints.size() / 2 );


	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ints.size() * sizeof( int ), ( void * ) ints.data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( int ), &count );
	program.AddKernelArgGlobal( CL_MEM_READ_WRITE , ( ints.size() / 2 )*sizeof( int ));
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( int ) );

	program.SetFirstWorkSize( ints.size() / 2 );


}
BPP_BENCHMARK
{
	program.Run();
}
BPP_RELEASE_BENCHMARK
{
	int output;
	program.ReadOutput( 3, &output );
	cout << "Output1:" << output << std::endl;
	program.Release();
}
BPP_END_BENCHMARK

BPP_BEGIN_BENCHMARK( OpenCL, GPU2 )
OpenCLProgram program;
BPP_INITIALIZE_BENCHMARK
{
	program = OpenCLProgram();
	program.InitializeCL();
	program.SelectPlatformAndDevice( selectedPlatformIdx,selectedDeviceIdx);
	program.LoadKernel( "Sum.cl", "sum2" );
	vector<int> ints;
	ints.resize( size );
	for( size_t i = 0; i < size; i++ )
	{
		ints[ i ] = i;
	}
	int count = ints.size();

	vector<int> tmp( ints.size() / 2 );

	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ints.size() * sizeof( int ), ( void * ) ints.data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( int ), &count );
	program.AddKernelArgGlobal( CL_MEM_READ_WRITE, ( ints.size() / 2 )*sizeof( int ));
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( int ) );

	program.SetFirstWorkSize( ints.size() / 2 );


}
BPP_BENCHMARK
{
	program.Run();
}
BPP_RELEASE_BENCHMARK
{
	int output;
	program.ReadOutput( 3, &output );
	cout << "Output2:" << output << std::endl;
	program.Release();
}
BPP_END_BENCHMARK

	BPP_BEGIN_BENCHMARK( OpenCL, GPU3 )
	OpenCLProgram program;
BPP_INITIALIZE_BENCHMARK
{
	program = OpenCLProgram();
	program.InitializeCL();
	program.SelectPlatformAndDevice( selectedPlatformIdx,selectedDeviceIdx);
	program.LoadKernel( "Sum.cl", "sum3" );
	vector<int> ints;
	ints.resize( size );
	for( size_t i = 0; i < size; i++ )
	{
		ints[ i ] = i;
	}
	int count = ints.size();

	vector<int> tmp( ints.size() / 2 );

	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ints.size() * sizeof( int ), ( void * ) ints.data() );
	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof( int ), &count );
	program.AddKernelArgLocal( ( ints.size() / 2 )*sizeof( int ));
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY, sizeof( int ) );

	program.SetFirstWorkSize( ints.size() / 2 );


}
BPP_BENCHMARK
{
	program.Run();
}
BPP_RELEASE_BENCHMARK
{
	int output;
	program.ReadOutput( 3, &output );
	cout << "Output3:" << output << std::endl;
	program.Release();
}
BPP_END_BENCHMARK


BPP_BEGIN_BENCHMARK( OpenCL, CPU )
vector<int> ints;
int sum;

BPP_INITIALIZE_BENCHMARK
{
	ints.resize( size );
	for( size_t i = 0; i < size; i++ )
	{
		ints[ i ] = i;
	}
}
BPP_BENCHMARK
{
	sum = 0;
	for( auto it : ints )
	{
		sum += it;
	}
}
BPP_RELEASE_BENCHMARK
{
	cout << "Expected Output:" << sum<< std::endl;
}
BPP_END_BENCHMARK