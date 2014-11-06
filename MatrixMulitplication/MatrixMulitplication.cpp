// MatrixMulitplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OpenCLProgram.h"


int _tmain(int argc, _TCHAR* argv[])
{

	OpenCLProgram program;

	program.InitializeCL();



	program.Release();


	return 0;
}

