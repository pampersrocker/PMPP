#pragma once
#ifndef OpenCLManager_inl__
#define OpenCLManager_inl__
#include "OpenCLManager.h"

inline
OpenCLManager::OpenCLManager():
	m_Platforms()
{

}

inline
OpenCLManager::~OpenCLManager()
{
	Release();
}

inline
void OpenCLManager::Release()
{

}


inline
void OpenCLManager::Initialize()
{
	cl_uint platformNumEntries = 10;
	cl_platform_id platformIds[ 10 ];
	memset( platformIds, 0, platformNumEntries*sizeof( cl_platform_id ) );
	cl_uint platformActualEntries = 0;
	CL_ASSERT( clGetPlatformIDs( platformNumEntries, platformIds, &platformActualEntries ) );

	for( cl_uint i = 0; i < platformActualEntries; i++ )
	{
		OpenCLPlatform* platform = new OpenCLPlatform( platformIds[ i ] );

		platform->LoadData();

		cl_uint deviceNumEntries = 10;
		cl_device_id deviceIds[ 10 ];
		memset( deviceIds, 0, deviceNumEntries*sizeof( cl_device_id ) );
		cl_uint deviceActualEntries = 0;
		CL_ASSERT( clGetDeviceIDs( platform->PlatformId(), CL_DEVICE_TYPE_ALL, deviceNumEntries, deviceIds, &deviceActualEntries ) );

		for( cl_uint i = 0; i < deviceActualEntries; i++ )
		{
			OpenCLDevice* device = new OpenCLDevice( platform, deviceIds[ i ] );
			device->LoadData();
			platform->AddDevice( device );
		}

		m_Platforms.push_back( platform );
	}
}

inline
const std::vector< OpenCLPlatform* >& OpenCLManager::Platforms()
{
	return m_Platforms;
}

#endif // OpenCLManager_inl__
