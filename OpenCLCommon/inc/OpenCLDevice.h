#pragma once
#include <CL\cl.h>
#include <string>
#include "ReferenceCounted.hpp"
class OpenCLPlatform;
class OpenCLContext;

class OpenCLDevice
{
public:

	OpenCLPlatform* Platform() const;
	cl_device_id CLDeviceId() const;

	ReferenceCounted< OpenCLContext > CreateContext();

	template <unsigned int IndexDimension>
	friend class OpenCLKernel_tpl;
	friend class OpenCLManager;
	friend class OpenCLPlatform;

	cl_uint cl_device_address_bits;
	cl_bool cl_device_available;
	cl_bool cl_device_compiler_available;
	cl_device_fp_config cl_device_double_fp_config;
	cl_bool cl_device_endian_little;
	cl_bool cl_device_error_correction_support;
	cl_device_exec_capabilities cl_device_execution_capabilities;
	std::string cl_device_extensions;
	cl_ulong cl_device_global_mem_cache_size;
	cl_device_mem_cache_type cl_device_global_mem_cache_type;
	cl_uint cl_device_global_mem_cacheline_size;
	cl_ulong cl_device_global_mem_size;
	//cl_device_fp_config cl_device_half_fp_config;
	cl_bool cl_device_host_unified_memory;
	cl_bool cl_device_image_support;
	size_t cl_device_image2d_max_height;
	size_t cl_device_image2d_max_width;
	size_t cl_device_image3d_max_depth;
	size_t cl_device_image3d_max_height;
	size_t cl_device_image3d_max_width;
	cl_ulong cl_device_local_mem_size;
	cl_device_local_mem_type CL_device_local_mem_type;
	cl_uint cl_device_max_clock_frequency;
	cl_uint cl_device_max_compute_units;
	cl_uint cl_device_max_constant_args;
	cl_ulong cl_device_max_constant_buffer_size;
	cl_ulong cl_device_max_mem_alloc_size;
	size_t cl_device_max_parameter_size;
	cl_uint cl_device_max_read_image_args;
	cl_uint cl_device_max_samplers;
	size_t cl_device_max_work_group_size;
	cl_uint cl_device_max_work_item_dimensions;
	//size_t* cl_device_max_work_item_sizes;
	cl_uint cl_device_max_write_image_args;
	cl_uint cl_device_mem_base_addr_align;
	cl_uint cl_device_min_data_type_align_size;
	std::string cl_device_name;
	cl_uint cl_device_native_vector_width_half;
	std::string cl_device_opencl_c_version;
	cl_platform_id cl_device_platform;
	cl_uint cl_device_preferred_vector_width_half;
	std::string cl_device_profile;
	size_t cl_device_profiling_timer_resolution;
	cl_command_queue_properties cl_device_queue_properties;
	cl_device_fp_config cl_device_single_fp_config;
	cl_device_type CL_device_type;
	std::string cl_device_vendor;
	cl_uint cl_device_vendor_id;
	std::string cl_device_version;
	std::string cl_driver_version;

protected:
private:
	OpenCLDevice( OpenCLPlatform* platform, cl_device_id deviceId );
	~OpenCLDevice();

	bool LoadData();

	template <typename T>
	void LoadData(T* dst, cl_device_info info);

	cl_device_id m_DeviceId;
	OpenCLPlatform* m_Platform;
};

template <typename T>
void OpenCLDevice::LoadData( T* dst, cl_device_info info )
{
	clGetDeviceInfo(m_DeviceId, info, sizeof(T), dst, nullptr);
}

template <>
inline void OpenCLDevice::LoadData<std::string>( std::string* dst, cl_device_info info )
{
	char tmp[256];
	memset( tmp, 0, 256 );
	clGetDeviceInfo(m_DeviceId, info, 256, tmp, nullptr);
	*dst = tmp;
}
