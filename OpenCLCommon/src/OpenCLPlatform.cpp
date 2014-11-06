#include "stdafx.h"
#include "OpenCLPlatform.h"
#include "OpenCLDevice.h"
#include <CL/cl.h>



OpenCLPlatform::OpenCLPlatform( cl_platform_id platformId ) :
	m_PlatformId(platformId)
{
}

void OpenCLPlatform::LoadData()
{
	char tmp[256];
	memset(tmp,0,256);
	clGetPlatformInfo( m_PlatformId, CL_PLATFORM_NAME, 256, tmp, nullptr );
	m_CLPlatformName = tmp;
	memset(tmp,0,256);
	clGetPlatformInfo( m_PlatformId, CL_PLATFORM_EXTENSIONS, 256, tmp, nullptr );
	m_CLPlatformExtensions = tmp;
	memset(tmp,0,256);
	clGetPlatformInfo( m_PlatformId, CL_PLATFORM_PROFILE, 256, tmp, nullptr );
	m_CLPlatformProfile = tmp;
	memset(tmp,0,256);
	clGetPlatformInfo( m_PlatformId, CL_PLATFORM_VENDOR, 256, tmp, nullptr );
	m_CLPlatformVendor = tmp;
	memset(tmp,0,256);
	clGetPlatformInfo( m_PlatformId, CL_PLATFORM_VERSION, 256, tmp, nullptr );
	m_CLPlatformVersion = tmp;
}

std::string OpenCLPlatform::PlatformExtensions() const
{
	return m_CLPlatformExtensions;
}

std::string OpenCLPlatform::PlatformVendor() const
{
	return m_CLPlatformVendor;
}

std::string OpenCLPlatform::PlatformName() const
{
	return m_CLPlatformName;
}

std::string OpenCLPlatform::PlatformVersion() const
{
	return m_CLPlatformVersion;
}

std::string OpenCLPlatform::PlatformProfile() const
{
	return m_CLPlatformProfile;
}

void OpenCLPlatform::AddDevice( OpenCLDevice* device )
{
	m_Devices.push_back(device);
}

OpenCLPlatform::~OpenCLPlatform()
{
	for (auto iter = m_Devices.begin(); iter != m_Devices.end(); ++iter)
	{
		delete *iter;
	}
	m_Devices.clear();
}

cl_platform_id OpenCLPlatform::PlatformId() const
{
	return m_PlatformId;
}
