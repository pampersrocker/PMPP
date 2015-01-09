#include "stdafx.h"
#include "OpenCLDevice.h"
#include "OpenCLContext.h"

OpenCLDevice::OpenCLDevice( OpenCLPlatform* platform, cl_device_id deviceId ) :
	m_Platform(platform),
	m_DeviceId(deviceId)
{

}

OpenCLDevice::~OpenCLDevice()
{

}

OpenCLPlatform* OpenCLDevice::Platform() const
{
	return m_Platform;
}

cl_device_id OpenCLDevice::CLDeviceId() const
{
	return m_DeviceId;
}

bool OpenCLDevice::LoadData()
{

	LoadData<cl_uint>(&cl_device_address_bits,CL_DEVICE_ADDRESS_BITS);
	LoadData<cl_bool>(&cl_device_available,CL_DEVICE_AVAILABLE);
	LoadData<cl_bool>(&cl_device_compiler_available,CL_DEVICE_COMPILER_AVAILABLE);
	//LoadData<cl_device_fp_config>(&cl_device_double_fp_config,CL_DEVICE_DOUBLE_FP_CONFIG);
	LoadData<cl_bool>(&cl_device_endian_little,CL_DEVICE_ENDIAN_LITTLE);
	LoadData<cl_bool>(&cl_device_error_correction_support,CL_DEVICE_ERROR_CORRECTION_SUPPORT);
	LoadData<cl_device_exec_capabilities>(&cl_device_execution_capabilities,CL_DEVICE_EXECUTION_CAPABILITIES);
	LoadData<std::string>(&cl_device_extensions,CL_DEVICE_EXTENSIONS);
	LoadData<cl_ulong>(&cl_device_global_mem_cache_size,CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
	LoadData<cl_device_mem_cache_type>(&cl_device_global_mem_cache_type,CL_DEVICE_GLOBAL_MEM_CACHE_TYPE);
	LoadData<cl_uint>(&cl_device_global_mem_cacheline_size,CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE);
	LoadData<cl_ulong>(&cl_device_global_mem_size,CL_DEVICE_GLOBAL_MEM_SIZE);
	//LoadData<cl_device_fp_config>(&cl_device_half_fp_config,CL_DEVICE_HALF_FP_CONFIG);
	LoadData<cl_bool>(&cl_device_host_unified_memory,CL_DEVICE_HOST_UNIFIED_MEMORY);
	LoadData<cl_bool>(&cl_device_image_support,CL_DEVICE_IMAGE_SUPPORT);
	LoadData<size_t>(&cl_device_image2d_max_height,CL_DEVICE_IMAGE2D_MAX_HEIGHT);
	LoadData<size_t>(&cl_device_image2d_max_width,CL_DEVICE_IMAGE2D_MAX_WIDTH);
	LoadData<size_t>(&cl_device_image3d_max_depth,CL_DEVICE_IMAGE3D_MAX_DEPTH);
	LoadData<size_t>(&cl_device_image3d_max_height,CL_DEVICE_IMAGE3D_MAX_HEIGHT);
	LoadData<size_t>(&cl_device_image3d_max_width,CL_DEVICE_IMAGE3D_MAX_WIDTH);
	LoadData<cl_ulong>(&cl_device_local_mem_size,CL_DEVICE_LOCAL_MEM_SIZE);
	LoadData<cl_device_local_mem_type>(&CL_device_local_mem_type,CL_DEVICE_LOCAL_MEM_TYPE);
	LoadData<cl_uint>(&cl_device_max_clock_frequency,CL_DEVICE_MAX_CLOCK_FREQUENCY);
	LoadData<cl_uint>(&cl_device_max_compute_units,CL_DEVICE_MAX_COMPUTE_UNITS);
	LoadData<cl_uint>(&cl_device_max_constant_args,CL_DEVICE_MAX_CONSTANT_ARGS);
	LoadData<cl_ulong>(&cl_device_max_constant_buffer_size,CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
	LoadData<cl_ulong>(&cl_device_max_mem_alloc_size,CL_DEVICE_MAX_MEM_ALLOC_SIZE);
	LoadData<size_t>(&cl_device_max_parameter_size,CL_DEVICE_MAX_PARAMETER_SIZE);
	LoadData<cl_uint>(&cl_device_max_read_image_args,CL_DEVICE_MAX_READ_IMAGE_ARGS);
	LoadData<cl_uint>(&cl_device_max_samplers,CL_DEVICE_MAX_SAMPLERS);
	LoadData<size_t>(&cl_device_max_work_group_size,CL_DEVICE_MAX_WORK_GROUP_SIZE);
	LoadData<cl_uint>(&cl_device_max_work_item_dimensions,CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
	//LoadData<size_t*>(&cl_device_max_work_item_sizes,CL_DEVICE_MAX_WORK_ITEM_SIZES);
	LoadData<cl_uint>(&cl_device_max_write_image_args,CL_DEVICE_MAX_WRITE_IMAGE_ARGS);
	LoadData<cl_uint>(&cl_device_mem_base_addr_align,CL_DEVICE_MEM_BASE_ADDR_ALIGN);
	LoadData<cl_uint>(&cl_device_min_data_type_align_size,CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE);
	LoadData<std::string>(&cl_device_name,CL_DEVICE_NAME);
	LoadData<cl_uint>(&cl_device_native_vector_width_half,CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF);
	LoadData<std::string>(&cl_device_opencl_c_version,CL_DEVICE_OPENCL_C_VERSION);
	LoadData<cl_platform_id>(&cl_device_platform,CL_DEVICE_PLATFORM);
	LoadData<cl_uint>(&cl_device_preferred_vector_width_half,CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF);
	LoadData<std::string>(&cl_device_profile,CL_DEVICE_PROFILE);
	LoadData<size_t>(&cl_device_profiling_timer_resolution,CL_DEVICE_PROFILING_TIMER_RESOLUTION);
	LoadData<cl_command_queue_properties>(&cl_device_queue_properties,CL_DEVICE_QUEUE_PROPERTIES);
	LoadData<cl_device_fp_config>(&cl_device_single_fp_config,CL_DEVICE_SINGLE_FP_CONFIG);
	LoadData<cl_device_type>(&CL_device_type,CL_DEVICE_TYPE);
	LoadData<std::string>(&cl_device_vendor,CL_DEVICE_VENDOR);
	LoadData<cl_uint>(&cl_device_vendor_id,CL_DEVICE_VENDOR_ID);
	LoadData<std::string>(&cl_device_version,CL_DEVICE_VERSION);
	LoadData<std::string>(&cl_driver_version,CL_DRIVER_VERSION);

	return true;
}

OpenCLContext* OpenCLDevice::CreateContext()
{
	return new OpenCLContext( this );
}
