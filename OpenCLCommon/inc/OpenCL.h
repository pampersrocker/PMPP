#pragma once

/**
Common Header for the complete OpenCLWrapper. 
This header can be included in the project and covers all relevant headers needed for the wrapper.
No other headers are required. (CL itself should be in your buildpath)
*/

#include "OpenCLCommon.h"
#include "Allocator.hpp"
#include "Allocator.inl"
#include "RefCountPolicy.hpp"
#include "OpenCLBuffer.h"
#include "OpenCLKernelArgument.h"
#include "OpenCLCommandQueue.h"
#include "OpenCLContext.h"
#include "OpenCLDevice.h"
#include "OpenCLPlatform.h"
#include "OpenCLKernel.h"
#include "OpenCLManager.h"