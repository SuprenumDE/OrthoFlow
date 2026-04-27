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
// Version 0.0.01, 06.03.2026
// 
// What does the RobustScaler do?
// 1. Uses median instead of mean
// 2. IQR instead of standard deviation
// 3. Outliers completely neutralized
// 4. Extremely useful for price data (diamonds!)
// ------------------------------------------------------


#include "RobustScaler.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;

constexpr double EPS = 1e-12;

// ---------------------------------------------------------
// Utility: Median
// ---------------------------------------------------------
double RobustScaler::compute_median(Eigen::VectorXd v) {
    std::sort(v.data(), v.data() + v.size());
    int n = v.size();
    return (n % 2 == 0)
        ? 0.5 * (v[n / 2 - 1] + v[n / 2])
        : v[n / 2];
}

// ---------------------------------------------------------
// Fit
// ---------------------------------------------------------
void RobustScaler::fit(const Eigen::MatrixXd& X) {
    int n = X.cols();
    median_.resize(n);
    iqr_.resize(n);

    for (int j = 0; j < n; ++j) {
        Eigen::VectorXd col = X.col(j);
        Eigen::VectorXd sorted = col;
        std::sort(sorted.data(), sorted.data() + sorted.size());

        median_(j) = compute_median(sorted);

        double q1 = sorted[int(0.25 * (sorted.size() - 1))];
        double q3 = sorted[int(0.75 * (sorted.size() - 1))];
        double iqr = q3 - q1;

        validate_fit_parameters(iqr, j);

        iqr_(j) = iqr;
    }
}

// ---------------------------------------------------------
// Transform
// ---------------------------------------------------------
Eigen::MatrixXd RobustScaler::transform(const Eigen::MatrixXd& X) const {
    Eigen::MatrixXd Xs = X;

    Xs = Xs.rowwise() - median_.transpose();
    Xs = Xs.array().rowwise() / iqr_.transpose().array();

    validate_transformed_matrix(Xs);

    return Xs;
}

// ---------------------------------------------------------
// Validation helpers
// ---------------------------------------------------------
void RobustScaler::validate_fit_parameters(double iqr, int col_index) const {
    if (iqr < EPS) {
        throw std::runtime_error(
            "RobustScaler::fit(): Feature column " + std::to_string(col_index) +
            " has near-zero IQR (" + std::to_string(iqr) + "). "
            "Scaling would lead to division by zero or numerical instability."
        );
    }
}

void RobustScaler::validate_transformed_matrix(const Eigen::MatrixXd& Xs) const {
    for (int i = 0; i < Xs.rows(); ++i) {
        for (int j = 0; j < Xs.cols(); ++j) {
            double v = Xs(i, j);
            if (!std::isfinite(v)) {
                throw std::runtime_error(
                    "RobustScaler::transform(): Non-finite value (NaN/Inf) "
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
void RobustScaler::save(const std::string& path) const {
    json j;
    j["type"] = "RobustScaler";
    j["n_features"] = median_.size();
    j["median"] = std::vector<double>(median_.data(), median_.data() + median_.size());
    j["iqr"] = std::vector<double>(iqr_.data(), iqr_.data() + iqr_.size());

    std::ofstream out(path);
    out << j.dump(4);
}

// ---------------------------------------------------------
// Load
// ---------------------------------------------------------
void RobustScaler::load(const std::string& path) {
    std::ifstream in(path);
    json j;
    in >> j;

    int n = j["n_features"];
    median_.resize(n);
    iqr_.resize(n);

    auto med_vec = j["median"].get<std::vector<double>>();
    auto iqr_vec = j["iqr"].get<std::vector<double>>();

    for (int i = 0; i < n; ++i) {
        median_(i) = med_vec[i];
        iqr_(i) = iqr_vec[i];
    }
}

