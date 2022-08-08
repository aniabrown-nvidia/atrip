// Copyright 2022 Alejandro Gallo
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// [[file:~/cuda/atrip/atrip.org::*Blas][Blas:2]]
#include <atrip/Blas.hpp>
#include <atrip/Atrip.hpp>

#if defined(HAVE_CUDA)
#  include <cstring>

  static inline
  cublasOperation_t char_to_cublasOperation(const char* trans) {
    if (strncmp("N", trans, 1) == 0)
      return CUBLAS_OP_N;
    else if (strncmp("T", trans, 1) == 0)
      return CUBLAS_OP_T;
    else
      return CUBLAS_OP_C;
  }

#endif

namespace atrip {


  template <>
  void xgemm<double>(const char *transa,
             const char *transb,
             const int *m,
             const int *n,
             const int *k,
             double *alpha,
             const typename DataField<double>::type *A,
             const int *lda,
             const typename DataField<double>::type *B,
             const int *ldb,
             double *beta,
             typename DataField<double>::type *C,
             const int *ldc) {
#if defined(HAVE_CUDA)
    cublasDgemm(Atrip::cuda.handle,
                char_to_cublasOperation(transa),
                char_to_cublasOperation(transb),
                *m, *n, *k,
                alpha, A, *lda,
                B, *ldb, beta,
                C, *ldc);
#else
    dgemm_(transa, transb,
           m, n, k,
           alpha, A, lda,
           B, ldb, beta,
           C, ldc);
#endif
  }

  template <>
  void xgemm<Complex>(const char *transa,
             const char *transb,
             const int *m,
             const int *n,
             const int *k,
             Complex *alpha,
             const typename DataField<Complex>::type *A,
             const int *lda,
             const typename DataField<Complex>::type *B,
             const int *ldb,
             Complex *beta,
             typename DataField<Complex>::type *C,
             const int *ldc) {
#if defined(HAVE_CUDA)
#pragma warning HAVE_CUDA
    cuDoubleComplex
      cu_alpha = {std::real(*alpha), std::imag(*alpha)},
      cu_beta = {std::real(*beta), std::imag(*beta)};
    cublasZgemm(Atrip::cuda.handle,
                char_to_cublasOperation(transa),
                char_to_cublasOperation(transb),
                *m, *n, *k,
                &cu_alpha,

                A, *lda,
                B, *ldb,
                &cu_beta,
                C, *ldc);
#else
    zgemm_(transa, transb,
           m, n, k,
           alpha, A, lda,
           B, ldb, beta,
           C, ldc);
#endif
  }


  template <>
  void xcopy<double>(const int n,
                     const  DataFieldType<double>* x,
                     const int incx,
                     DataFieldType<double>* y,
                     const int incy) {
#if defined(HAVE_CUDA)
    cublasDcopy(Atrip::cuda.handle,
                n,
                x, incx,
                y, incy);
#else
    dcopy_(n, x, incx, y, incy);
#endif
  }
 
  template <>
  void xcopy<Complex>(const int n,
                     const  DataFieldType<Complex>* x,
                     const int incx,
                     DataFieldType<Complex>* y,
                     const int incy) {
#if defined(HAVE_CUDA)
    cublasZcopy(Atrip::cuda.handle,
                n,
                x, incx,
                y, incy);
#else
    zcopy_(n, x, incx, y, incy);
#endif
  }


}
// Blas:2 ends here