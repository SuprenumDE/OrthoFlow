// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Jacobi SVD
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 10.03.2026
// ------------------------------------------------------


#include "JacobiSVDImpl.h"

namespace orthoflow {
    namespace decomposition {

        void JacobiSVDImpl::compute(const Matrix& X) {
            svd_.compute(X, Eigen::ComputeThinU | Eigen::ComputeThinV);
        }

        const JacobiSVDImpl::Vector& JacobiSVDImpl::singularValues() const {
            return svd_.singularValues();
        }

        const JacobiSVDImpl::Matrix& JacobiSVDImpl::matrixU() const {
            return svd_.matrixU();
        }

        const JacobiSVDImpl::Matrix& JacobiSVDImpl::matrixV() const {
            return svd_.matrixV();
        }

    } // namespace decomposition
} // namespace orthoflow
