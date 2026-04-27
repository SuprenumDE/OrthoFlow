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


#pragma once

#include "SVDBase.h"
#include <Eigen/Dense>

namespace orthoflow {
    namespace decomposition {

        class BDCSVDImpl : public SVDBase {
        public:
            using Matrix = Eigen::MatrixXd;
            using Vector = Eigen::VectorXd;

            BDCSVDImpl() = default;
            ~BDCSVDImpl() override = default;

            void compute(const Matrix& X) override;

            const Vector& singularValues() const override;
            const Matrix& matrixU() const override;
            const Matrix& matrixV() const override;

        private:
            Eigen::BDCSVD<Matrix> svd_;
        };

    } // namespace decomposition
} // namespace orthoflow
