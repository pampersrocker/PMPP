#pragma once
#include <CL\cl.h>
#include <string>
class OpenCLPlatform;

class OpenCLDevice
{
public:

	OpenCLDevice(OpenCLPlatform* platform, cl_device_id deviceId);
	OpenCLPlatform* Platform() const;
	cl_device_id DeviceId() const;

	bool LoadData();
	friend class OpenCLProgram;

protected:
private:
	template <typename T>
	void LoadData(T* dst,cl_device_info info);

	cl_device_id m_DeviceId;
	OpenCLPlatform* m_Platform;
};

template <typename T>
void OpenCLDevice::LoadData( T* dst,cl_device_info info )
{
	clGetDeviceInfo(m_DeviceId, info, sizeof(T), dst, nullptr);
}

template <>
inline void OpenCLDevice::LoadData<std::string>( std::string* dst,cl_device_info info )
{
	char tmp[256];
	memset(tmp,0,256);
	clGetDeviceInfo(m_DeviceId, info, 256, tmp, nullptr);
	*dst = tmp;
}
