// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow RobustScaler
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

class RobustScaler : public BaseScaler {
public:
    RobustScaler() = default;

    void fit(const Eigen::MatrixXd& X) override;
    Eigen::MatrixXd transform(const Eigen::MatrixXd& X) const override;

    void save(const std::string& path) const override;
    void load(const std::string& path) override;

private:
    Eigen::VectorXd median_;
    Eigen::VectorXd iqr_;

    // Internal validation helpers
    void validate_fit_parameters(double iqr, int col_index) const;
    void validate_transformed_matrix(const Eigen::MatrixXd& Xs) const;

    // Utility
    static double compute_median(Eigen::VectorXd v);
};

