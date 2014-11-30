// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "BenchmarkPrefixSumScenario.h"
#define BPP_CUSTOM_SCENARIO ::PrefixSumScenario
#include "bpp.hpp"

#include "OpenCLProgram.h"
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"



// TODO: reference additional headers your program requires here
