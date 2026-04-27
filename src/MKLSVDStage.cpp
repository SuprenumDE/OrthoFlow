// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow MKLSVDStage
// MKL integration via Intel oneAPI, see also FlowEngine.h
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 02.04.2026
// 
/* The routine LAPACKE_dgesdd computes the singular value decomposition (SVD) 
of a real m - by - n matrix A, optionally computing the left and /or right singular
vectors.If singular vectors are desired, it uses a divide and conquer
algorithm.The SVD is written as

                       A = U * SIGMA * VT

where SIGMA is an m - by - n matrix which is zero except for its min(m, n)
diagonal elements, U is an m - by - m orthogonal matrix and VT(V transposed)
is an n - by - n orthogonal matrix.The diagonal elements of SIGMA
are the singular values of A; they are real and non - negative, and are
returned in descending order.The first min(m, n) columns of U and V are
the left and right singular vectors of A.

Note that the routine returns VT, not V.*/

// ------------------------------------------------------


#include "MKLSVDStage.h"
#include <mkl.h>
#include <stdexcept>

namespace orthoflow {
    namespace decomposition {

        void MKLSVDStage::compute(const Matrix& X)
        {
            const MKL_INT m = static_cast<MKL_INT>(X.rows());
            const MKL_INT n = static_cast<MKL_INT>(X.cols());
            const MKL_INT k = std::min(m, n);

            // Copy input matrix (LAPACK works in-place)
            Matrix A = X;

            singular_values_.resize(k);
            U_.resize(m, k);
            V_.resize(k, n);

            // LAPACKE expects column-major (Eigen default)
            MKL_INT lda = m;
            MKL_INT ldu = m;
            MKL_INT ldvt = k;

            // dgesdd workspace query
            MKL_INT info;
            MKL_INT lwork = -1;
            double wkopt;

            info = LAPACKE_dgesdd_work(
                LAPACK_COL_MAJOR,
                'S',            // thin SVD
                m, n,
                A.data(), lda,
                singular_values_.data(),
                U_.data(), ldu,
                V_.data(), ldvt,
                &wkopt, lwork,
                nullptr
            );

            if (info != 0)
                throw std::runtime_error("MKL dgesdd workspace query failed");

            lwork = static_cast<MKL_INT>(wkopt);
            std::vector<double> work(lwork);

            // iwork array (size 8*min(m,n))
            std::vector<MKL_INT> iwork(8 * k);

            // actual SVD
            info = LAPACKE_dgesdd_work(
                LAPACK_COL_MAJOR,
                'S',
                m, n,
                A.data(), lda,
                singular_values_.data(),
                U_.data(), ldu,
                V_.data(), ldvt,
                work.data(), lwork,
                iwork.data()
            );

            if (info > 0)
                throw std::runtime_error("MKL dgesdd: SVD did not converge");
            if (info < 0)
                throw std::runtime_error("MKL dgesdd: illegal argument");

            // Transpose:
            V_.transposeInPlace();


        }

    } // namespace decomposition
} // namespace orthoflow
