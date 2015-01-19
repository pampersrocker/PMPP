#pragma once
#ifndef MatrixMultScenario_h__
#define MatrixMultScenario_h__

#include "OpenCL.h"

class MatrixMultScenario
{
public:

	ReferenceCounted< OpenCLKernel_tpl<1> > kernel;
	ReferenceCounted< OpenCLKernel_tpl<2> > kernelShared;
	ReferenceCounted< OpenCLKernel_tpl<2> > kernelSharedSafe;
	ReferenceCounted< OpenCLKernel_tpl<2> > kernelSharedTransposed;

	std::string ToString() const;

protected:
private:
};

inline
std::string MatrixMultScenario::ToString() const
{
	return "Default Scenario";
}

#endif // MatrixMultScenario_h__
