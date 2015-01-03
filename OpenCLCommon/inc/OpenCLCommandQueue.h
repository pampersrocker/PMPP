#pragma once
#ifndef OpenCLCommandQueue_h__
#define OpenCLCommandQueue_h__

class OpenCLContext;

class OpenCLCommandQueue
{
public:
	OpenCLCommandQueue( OpenCLContext* parentContext );
	~OpenCLCommandQueue();

	OpenCLContext* Context() const;
	cl_command_queue CommandQueue() const;

private:

	OpenCLContext* m_Context;
	cl_command_queue m_CommandQueue;
};

#endif // OpenCLCommandQueue_h__
