#include "OpenCLDevice.h"

OpenCLDevice::OpenCLDevice( OpenCLPlatform* platform, cl_device_id deviceId ) :
	m_Platform(platform),
	m_DeviceId(deviceId)
{

}

OpenCLPlatform* OpenCLDevice::Platform() const
{
	return m_Platform;
}

cl_device_id OpenCLDevice::DeviceId() const
{
	return m_DeviceId;
}

bool OpenCLDevice::LoadData()
{
	return true;
}
