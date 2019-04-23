#pragma once

#include <sstream>
#include <complex>

#include <cuda_runtime.h>

#include "../global/global.h"

using float_t = global::float_t;
using float_ptr = std::unique_ptr<float_t[]>;
using complex = std::complex<float_t>;

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
template<typename T>
void copy_to_device(const T* host, T* device, size_t size)
{
	auto err = cudaMemcpy(device, host, size*sizeof(T), cudaMemcpyHostToDevice);
	CHECK
}
//����device�Դ����ݵ�host��
template<typename T>
void copy_to_host(T* device, T* host, size_t size)
{
	auto err = cudaMemcpy(device, host, size*sizeof(T), cudaMemcpyDeviceToHost);
	CHECK
}