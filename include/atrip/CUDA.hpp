#pragma once

#include <atrip/Utils.hpp>

#if defined(HAVE_CUDA)
#include <cuda.h>
#endif

#include <sstream>

#if defined(HAVE_CUDA) && defined(__CUDACC__)
#  define __MAYBE_GLOBAL__ __global__
#  define __MAYBE_DEVICE__ __device__
#else
#  define __MAYBE_GLOBAL__
#  define __MAYBE_DEVICE__
#endif

#define _CHECK_CUDA_SUCCESS(message, ...)                               \
  do {                                                                  \
    CUresult result = __VA_ARGS__;                                      \
    if (result != CUDA_SUCCESS) {                                       \
      auto msg = _FORMAT("\t!!CUDA_ERROR(%d): %s:%d\v%s",               \
                         result,                                        \
                         __FILE__,                                      \
                         __LINE__,                                      \
                         message);                                      \
      std::cerr << msg;                                                 \
      throw msg;                                                        \
    }                                                                   \
  } while (0)

#define _CHECK_CUBLAS_SUCCESS(message, ...)                             \
  do {                                                                  \
    cublasStatus_t result = __VA_ARGS__;                                      \
    if (result != 0) {                                                  \
      auto msg = _FORMAT("\t!!CUBLAS_ERROR(%d): %s:%d\v%s",             \
                         result,                                        \
                         __FILE__,                                      \
                         __LINE__,                                      \
                         message);                                      \
      std::cerr << msg;                                                 \
      throw msg;                                                        \
    }                                                                   \
  } while (0)
