#pragma once
#ifndef OpenCLContext_h__
#define OpenCLContext_h__

class OpenCLContext
{
public:
	OpenCLContext( OpenCLDevice* parentDevice );
	~OpenCLContext();

	OpenCLDevice* Device() const;

	cl_context Context() const;

private:

	OpenCLDevice* m_Device;

	cl_context m_Context;
};

#endif // OpenCLContext_h__
