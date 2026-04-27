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
// ------------------------------------------------------


#pragma once

#include "SVDBase.h"
#include <Eigen/Dense>

namespace orthoflow {
    namespace decomposition {

        class MKLSVDStage : public SVDBase {
        public:
            MKLSVDStage() = default;

            void compute(const Matrix& X) override;

            const Vector& singularValues() const override { return singular_values_; }
            const Matrix& matrixU() const override { return U_; }
            const Matrix& matrixV() const override { return V_; }

        private:
            Vector singular_values_;
            Matrix U_;   // thin U
            Matrix V_;   // thin V
        };

    } // namespace decomposition
} // namespace orthoflow

