// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow MinMax Scaler
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 10.03.2026
// ------------------------------------------------------


#pragma once
#include <Eigen/Dense>
#include <string>
#include "BaseScaler.h"

class MinMaxScaler : public BaseScaler {
public:
    MinMaxScaler() = default;

    void fit(const Eigen::MatrixXd& X) override;
    Eigen::MatrixXd transform(const Eigen::MatrixXd& X) const override;

    void save(const std::string& path) const override;
    void load(const std::string& path) override;

private:
    Eigen::VectorXd min_;
    Eigen::VectorXd max_;

    // Internal validation helpers
    void validate_fit_parameters(double range, int col_index) const;
    void validate_transformed_matrix(const Eigen::MatrixXd& Xs) const;
};

