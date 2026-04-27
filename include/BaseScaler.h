// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow BaseScaler
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 06.03.2026
// ------------------------------------------------------


#pragma once
#include <Eigen/Dense>
#include <string>

class BaseScaler {
public:
    virtual ~BaseScaler() = default;

    // Trains the scaler on the data:
    virtual void fit(const Eigen::MatrixXd& X) = 0;

    // Transforms the data:
    virtual Eigen::MatrixXd transform(const Eigen::MatrixXd& X) const = 0;

    // Persistence:
    virtual void save(const std::string& path) const = 0;
    virtual void load(const std::string& path) = 0;
};
