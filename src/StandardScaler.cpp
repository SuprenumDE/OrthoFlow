// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Standard Scaler
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 10.03.2026
// 
// What does the standard sclaer do?
// 1. Centered on mean
// 2. Scaled to standard deviation
// 3. Good for: linear models, PCA, SVD - based pipelines
// 4.Sensitive to outliers
// ------------------------------------------------------


#include "StandardScaler.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;

constexpr double EPS = 1e-12;

// ---------------------------------------------------------
// Fit
// ---------------------------------------------------------
void StandardScaler::fit(const Eigen::MatrixXd& X) {
    mean_ = X.colwise().mean();
    std_.resize(X.cols());

    for (int j = 0; j < X.cols(); ++j) {
        double variance = (X.col(j).array() - mean_(j)).square().mean();
        double s = std::sqrt(variance);

        validate_fit_parameters(variance, j);

        std_(j) = s;
    }
}

// ---------------------------------------------------------
// Transform
// ---------------------------------------------------------
Eigen::MatrixXd StandardScaler::transform(const Eigen::MatrixXd& X) const {
    Eigen::MatrixXd Xs = X;

    Xs = Xs.rowwise() - mean_.transpose();
    Xs = Xs.array().rowwise() / std_.transpose().array();

    validate_transformed_matrix(Xs);

    
    return Xs;
}

// ---------------------------------------------------------
// Validation helpers
// ---------------------------------------------------------
void StandardScaler::validate_fit_parameters(double variance, int col_index) const {
    if (variance < EPS) {
        throw std::runtime_error(
            "StandardScaler::fit(): Feature column " + std::to_string(col_index) +
            " has near-zero variance (" + std::to_string(variance) + "). "
            "Scaling would lead to numerical instability."
        );
    }
}

void StandardScaler::validate_transformed_matrix(const Eigen::MatrixXd& Xs) const {
    for (int i = 0; i < Xs.rows(); ++i) {
        for (int j = 0; j < Xs.cols(); ++j) {
            double v = Xs(i, j);
            if (!std::isfinite(v)) {
                throw std::runtime_error(
                    "StandardScaler::transform(): Non-finite value (NaN/Inf) "
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
void StandardScaler::save(const std::string& path) const {
    json j;
    j["type"] = "StandardScaler";
    j["n_features"] = mean_.size();
    j["mean"] = std::vector<double>(mean_.data(), mean_.data() + mean_.size());
    j["std"] = std::vector<double>(std_.data(), std_.data() + std_.size());

    std::ofstream out(path);
    out << j.dump(4);
}

// ---------------------------------------------------------
// Load
// ---------------------------------------------------------
void StandardScaler::load(const std::string& path) {
    std::ifstream in(path);
    json j;
    in >> j;

    int n = j["n_features"];
    mean_.resize(n);
    std_.resize(n);

    auto mean_vec = j["mean"].get<std::vector<double>>();
    auto std_vec = j["std"].get<std::vector<double>>();

    for (int i = 0; i < n; ++i) {
        mean_(i) = mean_vec[i];
        std_(i) = std_vec[i];
    }
}
