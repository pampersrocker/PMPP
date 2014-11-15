#pragma once
#include <string>
#include <CL/cl.h>
#include <vector>

class OpenCLDevice;
class OpenCLPlatform
{
public:

	OpenCLPlatform(cl_platform_id platformId);

	std::string PlatformProfile() const;
	std::string PlatformVersion() const;
	std::string PlatformName() const;
	std::string PlatformVendor() const;
	std::string PlatformExtensions() const;
	cl_platform_id PlatformId() const;


	const std::vector<OpenCLDevice*>& Devices() const { return m_Devices; }

	~OpenCLPlatform();
	template <unsigned int IndexDimension>
	friend class OpenCLProgram_tpl;
protected:
private:

	void AddDevice(OpenCLDevice* device);
	void LoadData();

	std::vector<OpenCLDevice*> m_Devices;
	cl_platform_id m_PlatformId;
	std::string m_CLPlatformProfile;
	std::string m_CLPlatformVersion;
	std::string m_CLPlatformName;
	std::string m_CLPlatformVendor;
	std::string m_CLPlatformExtensions;
};