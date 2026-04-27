// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow BDCSVD 
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 10.03.2026
// ------------------------------------------------------

#include "BDCSVDImpl.h"

namespace orthoflow {
    namespace decomposition {

        void BDCSVDImpl::compute(const Matrix& X) {
            svd_.compute(X, Eigen::ComputeThinU | Eigen::ComputeThinV);
        }

        const BDCSVDImpl::Vector& BDCSVDImpl::singularValues() const {
            return svd_.singularValues();
        }

        const BDCSVDImpl::Matrix& BDCSVDImpl::matrixU() const {
            return svd_.matrixU();
        }

        const BDCSVDImpl::Matrix& BDCSVDImpl::matrixV() const {
            return svd_.matrixV();
        }

    } // namespace decomposition
} // namespace orthoflow
