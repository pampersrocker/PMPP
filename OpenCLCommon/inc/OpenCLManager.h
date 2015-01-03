#pragma once
#ifndef OpenCLManager_h__
#define OpenCLManager_h__

#include "stdafx.h"

class OpenCLPlatform;

class OpenCLManager
{
public:
	OpenCLManager();

	void Initialize();

	const std::vector< OpenCLPlatform* >& Platforms();

	void Release();

	~OpenCLManager();

private:

	std::vector< OpenCLPlatform* > m_Platforms;

	cl_uint m_CurrentStatus;

	OpenCLManager( const OpenCLManager& ) = delete;

};

#include "OpenCLManager.inl"


#endif // OpenCLManager_h__
