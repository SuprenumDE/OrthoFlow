// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow MaxAbsScaler
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 10.03.2026
// 
// What does the MaxAbsScaler do?
// 1. Scales each feature to [-1, 1]
// 2. Works without centering
// 3. Preserves sparsity (important for one-hot encoded data)
// 3.a Perfect for sparse matrices (NLP or one-hot)
// 4. Extremely simple: only max(abs(x))
// 5. Useful for linear models
// ------------------------------------------------------

#include "MaxAbsScaler.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include "json.hpp"

using json = nlohmann::json;

constexpr double EPS = 1e-12;

// ---------------------------------------------------------
// Fit
// ---------------------------------------------------------
void MaxAbsScaler::fit(const Eigen::MatrixXd& X) {
    int n = X.cols();
    max_abs_.resize(n);

    for (int j = 0; j < n; ++j) {
        double m = X.col(j).cwiseAbs().maxCoeff();

        validate_fit_parameters(m, j);

        max_abs_(j) = m;
    }
}

// ---------------------------------------------------------
// Transform
// ---------------------------------------------------------
Eigen::MatrixXd MaxAbsScaler::transform(const Eigen::MatrixXd& X) const {
    Eigen::MatrixXd Xs = X;

    Xs = Xs.array().rowwise() / max_abs_.transpose().array();

    validate_transformed_matrix(Xs);

    return Xs;
}

// ---------------------------------------------------------
// Validation helpers
// ---------------------------------------------------------
void MaxAbsScaler::validate_fit_parameters(double maxAbs, int col_index) const {
    if (maxAbs < EPS) {
        throw std::runtime_error(
            "MaxAbsScaler::fit(): Feature column " + std::to_string(col_index) +
            " has near-zero maxAbs (" + std::to_string(maxAbs) + "). "
            "Scaling would lead to division by zero or numerical instability."
        );
    }
}

void MaxAbsScaler::validate_transformed_matrix(const Eigen::MatrixXd& Xs) const {
    for (int i = 0; i < Xs.rows(); ++i) {
        for (int j = 0; j < Xs.cols(); ++j) {
            double v = Xs(i, j);
            if (!std::isfinite(v)) {
                throw std::runtime_error(
                    "MaxAbsScaler::transform(): Non-finite value (NaN/Inf) "
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
void MaxAbsScaler::save(const std::string& path) const {
    json j;
    j["type"] = "MaxAbsScaler";
    j["n_features"] = max_abs_.size();
    j["max_abs"] = std::vector<double>(max_abs_.data(), max_abs_.data() + max_abs_.size());

    std::ofstream out(path);
    out << j.dump(4);
}

// ---------------------------------------------------------
// Load
// ---------------------------------------------------------
void MaxAbsScaler::load(const std::string& path) {
    std::ifstream in(path);
    json j;
    in >> j;

    int n = j["n_features"];
    max_abs_.resize(n);

    auto vec = j["max_abs"].get<std::vector<double>>();
    for (int i = 0; i < n; ++i) {
        max_abs_(i) = vec[i];
    }
}
