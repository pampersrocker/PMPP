#pragma once
#include "OpenCLBuffer.h"
#ifndef OpenCLKernelArgument_h__
#define OpenCLKernelArgument_h__

enum class OpenCLKernelArgumentType
{
	Global,
	Local,
	Value
};

class OpenCLKerneArgument
{
public:

	OpenCLKerneArgument();
	~OpenCLKerneArgument();
	OpenCLKernelArgumentType Type() const;
	OpenCLBufferPtr Buffer() const;

	template<typename T>
	void SetValue( T value );


protected:
private:

	template<typename T>
	void SetValueInternal( T value );

	OpenCLKernelArgumentType m_Type;
	OpenCLBufferPtr m_Buffer;

	size_t m_Size;

	union
	{
		cl_uchar Data;
		cl_float Float;
		cl_uint UInt;
		cl_int Int;
		cl_uchar4 UChar4;
		cl_float4 Float4;
		cl_uint4 UInt4;
	};
};

template<typename T>
void OpenCLKerneArgument::SetValueInternal( T value )
{
	static_assert( false, "Unsupported Type for SetValue!" );

}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_float >( cl_float value )
{
	Float = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< float >( float value )
{
	Float = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< int >( int value )
{
	Int = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< unsigned int >( unsigned int value )
{
	UInt = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uint >( cl_uint value )
{
	UInt = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_int >( cl_int value )
{
	Int = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uchar4 >( cl_uchar4 value )
{
	UChar4 = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_float4 >( cl_float4 value )
{
	Float4 = value;
}

template<>
inline
void OpenCLKerneArgument::SetValueInternal< cl_uint4 >( cl_uint4 value )
{
	UInt4 = value;
}

template<typename T>
inline
void OpenCLKerneArgument::SetValue( T value )
{
	m_Size = sizeof( T );
	SetValueInternal<T>( value );
}

#endif // OpenCLKernelArgument_h__
