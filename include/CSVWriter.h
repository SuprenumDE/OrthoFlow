// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow CSV Writer for SVD Projection 
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 20.03.2026
// ------------------------------------------------------


#pragma once
#include <Eigen/Dense>
#include <fstream>
#include <string>

#pragma once
#include <Eigen/Dense>
#include <fstream>
#include <string>

class CSVWriter {
public:
    static void write_yX(const std::string& filename,
        const Eigen::VectorXd& y,
        const Eigen::MatrixXd& X,
        char delimiter = ',')
    {
        if (y.size() != X.rows()) {
            throw std::runtime_error("CSVWriter: y and X row count mismatch.");
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("CSVWriter: Cannot open file: " + filename);
        }

        const Eigen::Index rows = X.rows();
        const Eigen::Index cols = X.cols();

        for (Eigen::Index i = 0; i < rows; ++i) {
            // Write label
            file << y(i) << delimiter;

            // Write features
            for (Eigen::Index j = 0; j < cols; ++j) {
                file << X(i, j);
                if (j < cols - 1)
                    file << delimiter;
            }
            file << "\n";
        }

        file.close();
    }
};
