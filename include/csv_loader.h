// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// Load data (CSV format)
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 25.01.2026
// ------------------------------------------------------

#pragma once
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Logger.h"

// ------------------------------------------------------------
//                Structured report for the caller
// ------------------------------------------------------------
struct CSVLoadIssue {
    size_t line;
    std::string message;
    std::string offending_value;
};

struct CSVLoadReport {
    size_t total_rows_read = 0;
    size_t total_rows_loaded = 0;
    size_t total_rows_dropped = 0;
    size_t expected_columns = 0;

    std::vector<CSVLoadIssue> issues;

    bool has_errors() const {
        return !issues.empty();
    }
};

// ------------------------------------------------------------
//                        CSVLoader
// ------------------------------------------------------------
class CSVLoader {
public:
    CSVLoader(Logger* logger = nullptr, bool strict_mode = true);

    Eigen::MatrixXd load(
        const std::string& path,
        char delimiter,
        CSVLoadReport* report
    );

private:
    Logger* logger_;
    bool strict_mode_;

    std::string_view trim(std::string_view sv);
};
