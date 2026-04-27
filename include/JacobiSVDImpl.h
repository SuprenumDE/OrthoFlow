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


#pragma once

#include "SVDBase.h"
#include <Eigen/Dense>

namespace orthoflow {
    namespace decomposition {

        class JacobiSVDImpl : public SVDBase {
        public:
            using Matrix = Eigen::MatrixXd;
            using Vector = Eigen::VectorXd;

            JacobiSVDImpl() = default;
            ~JacobiSVDImpl() override = default;

            void compute(const Matrix& X) override;

            const Vector& singularValues() const override;
            const Matrix& matrixU() const override;
            const Matrix& matrixV() const override;

        private:
            Eigen::JacobiSVD<Matrix> svd_;
        };

    } // namespace decomposition
} // namespace orthoflow
