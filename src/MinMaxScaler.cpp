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
// Version 0.0.01, 06.03.2026
// 
// What does the MinMax sclaer do?
// 1. Scaled to [0,1]
// 2. Good for: ReLU networks, visualizations, bounded features
// 3. Sensitive to outliers
// ------------------------------------------------------


#include "MinMaxScaler.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;

constexpr double EPS = 1e-12;

// ---------------------------------------------------------
// Fit
// ---------------------------------------------------------
void MinMaxScaler::fit(const Eigen::MatrixXd& X) {
    min_ = X.colwise().minCoeff();
    max_ = X.colwise().maxCoeff();

    for (int j = 0; j < X.cols(); ++j) {
        double range = max_(j) - min_(j);
        validate_fit_parameters(range, j);
    }
}

// ---------------------------------------------------------
// Transform
// ---------------------------------------------------------
Eigen::MatrixXd MinMaxScaler::transform(const Eigen::MatrixXd& X) const {
    Eigen::MatrixXd Xs = X;

    Xs = Xs.rowwise() - min_.transpose();
    Xs = Xs.array().rowwise() / (max_ - min_).transpose().array();

    validate_transformed_matrix(Xs);

    return Xs;
}

// ---------------------------------------------------------
// Validation helpers
// ---------------------------------------------------------
void MinMaxScaler::validate_fit_parameters(double range, int col_index) const {
    if (range < EPS) {
        throw std::runtime_error(
            "MinMaxScaler::fit(): Feature column " + std::to_string(col_index) +
            " has near-zero range (" + std::to_string(range) + "). "
            "Scaling would lead to division by zero or numerical instability."
        );
    }
}

void MinMaxScaler::validate_transformed_matrix(const Eigen::MatrixXd& Xs) const {
    for (int i = 0; i < Xs.rows(); ++i) {
        for (int j = 0; j < Xs.cols(); ++j) {
            double v = Xs(i, j);
            if (!std::isfinite(v)) {
                throw std::runtime_error(
                    "MinMaxScaler::transform(): Non-finite value (NaN/Inf) "
                    "encountered at row " + std::to_string(i) +
                    ", column " + std::to_string(j) + "."
                );
            }
        }
    }
}

// ---------------------------------------------------------
// Save
// ---------------------------------------------------------
void MinMaxScaler::save(const std::string& path) const {
    json j;
    j["type"] = "MinMaxScaler";
    j["n_features"] = min_.size();
    j["min"] = std::vector<double>(min_.data(), min_.data() + min_.size());
    j["max"] = std::vector<double>(max_.data(), max_.data() + max_.size());

    std::ofstream out(path);
    out << j.dump(4);
}

// ---------------------------------------------------------
// Load
// ---------------------------------------------------------
void MinMaxScaler::load(const std::string& path) {
    std::ifstream in(path);
    json j;
    in >> j;

    int n = j["n_features"];
    min_.resize(n);
    max_.resize(n);

    auto min_vec = j["min"].get<std::vector<double>>();
    auto max_vec = j["max"].get<std::vector<double>>();

    for (int i = 0; i < n; ++i) {
        min_(i) = min_vec[i];
        max_(i) = max_vec[i];
    }
}
