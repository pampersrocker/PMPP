// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <SFML/Graphics.hpp>
#include <OpenCL.h>
#include "HistogramGPUScenario.h"
#define BPP_CUSTOM_SCENARIO ::HistogramGPUScenario
#include <bpp.hpp>

typedef ReferenceCounted< OpenCLKernel_tpl < 1U > > OpenCLKernelPtr;


// TODO: reference additional headers your program requires here
