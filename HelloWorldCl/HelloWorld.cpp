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
#include "OpenCLProgram.h"

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

int main(int argc, char* argv[])
{

	OpenCLProgram program;
	program.Initialize( "HelloWorld_Kernel.cl", "helloworld" );
	string hello = "Hello World";
	char tmp[ 20 ];
	program.AddKernelArg( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, ( hello.size() + 1 )*sizeof( char ),(void *) hello.data());
	program.AddKernelArg( CL_MEM_WRITE_ONLY, ( hello.size() + 1 )*sizeof( char ));

	program.Run();

	program.ReadOutput( 1, tmp );

	tmp[ hello.size() ] = '\0';

	cout << "Input is: " << hello << std::endl;

	cout << "Output is: " << tmp << std::endl;


	program.Release();

	Benchmarker& benchmarker = Benchmarker::Instance();
}

BPP_BEGIN_BENCHMARK( OpenCL, CPU)
BPP_BENCHMARK
{

}
BPP_END_BENCHMARK( OpenCL, CPU )


BPP_BEGIN_BENCHMARK( OpenCL, GPU )
BPP_BENCHMARK
{

}
BPP_END_BENCHMARK( OpenCL, CPU )