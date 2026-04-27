// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Saving SVD results in binary format 
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 19.03.2026
// ------------------------------------------------------


#pragma once

#include <Eigen/Dense>
#include <string>

#include "zlib.h" 

#pragma warning(push)
#pragma warning(disable : 4996)
#include "cnpy.h"
#pragma warning(pop)

namespace orthoflow {
    namespace io {

        // ============================
        // Save Eigen::VectorXd as .npy
        // ============================
        inline void save_vector_npy(const std::string& filename,
            const Eigen::VectorXd& v)
        {
            cnpy::npy_save(
                filename,
                v.data(),
                { static_cast<size_t>(v.size()) },
                "w"
            );
        }

        // ============================
        // Save Eigen::MatrixXd as .npy
        // ============================
        inline void save_matrix_npy(const std::string& filename,
            const Eigen::MatrixXd& M)
        {
            cnpy::npy_save(
                filename,
                M.data(),
                { static_cast<size_t>(M.rows()),
                  static_cast<size_t>(M.cols()) },
                "w"
            );
        }

        // ============================
        // Load .npy into Eigen::VectorXd
        // ============================
        inline Eigen::VectorXd load_vector_npy(const std::string& filename)
        {
            cnpy::NpyArray arr = cnpy::npy_load(filename);
            double* data = arr.data<double>();
            size_t n = arr.shape[0];

            return Eigen::Map<Eigen::VectorXd>(data, n);
        }

        // ============================
        // Load .npy into Eigen::MatrixXd
        // ============================
        inline Eigen::MatrixXd load_matrix_npy(const std::string& filename)
        {
            cnpy::NpyArray arr = cnpy::npy_load(filename);
            double* data = arr.data<double>();

            size_t rows = arr.shape[0];
            size_t cols = arr.shape[1];

            // RowMajor Mapping, da cnpy row-major speichert
            return Eigen::Map<
                Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
            >(data, rows, cols);
        }

    } // namespace io
} // namespace orthoflow

