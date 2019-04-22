#pragma once

#include <sstream>

#include <cuda_runtime.h>

#include ""
using float_t = global::float_t;
using floath_ptr = std::unique_ptr<float_t[]>;

//cuda���������
#define CHECK_CUDA_ERROR(err)\
	if(err!=cudaSuccess)\
	{\
		std::stringstream msg;\
		msg << "cuda error: \n";\
		msg << cudaGetErrorName(err) << '\n';\
		msg << "at line: " << __LINE__;\
		throw std::runtime_error(msg.str());\
	}
//��err���д����飬�趨��errΪcudaError_t
#define CHECK CHECK_CUDA_ERROR(err)
//����host�ڴ����ݵ�device��
void copy_to_device(void* host, void* device, size_t size)
{
	auto err = cudaMemcpy(device, host, size, cudaMemcpyHostToDevice);
	CHECK
}
//����device�Դ����ݵ�host��
void copy_to_host(void* device, const void* host, size_t size)
{
	auto err = cudaMemcpy(device, host, size, cudaMemcpyDeviceToHost);
	CHECK
}