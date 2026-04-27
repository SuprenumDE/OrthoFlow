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
// Version 0.0.03, 26.01.2026
// ------------------------------------------------------

#include "csv_loader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// ------------------------------------------------------------
//  Centralized row-drop helper
// ------------------------------------------------------------
inline void drop_row(
    size_t line_number,
    const std::string& reason,
    CSVLoadReport* report,
    Logger* logger
) {
    if (report) {
        report->total_rows_dropped++;
        report->issues.push_back({ line_number, reason, "" });
    }
    if (logger) {
        logger->warning(
            "CSVLoader: Dropping row " +
            std::to_string(line_number) +
            " (" + reason + ")"
        );
    }
}

// ------------------------------------------------------------
//  Constructor
// ------------------------------------------------------------
CSVLoader::CSVLoader(Logger* logger, bool strict_mode)
    : logger_(logger), strict_mode_(strict_mode) {
}

// ------------------------------------------------------------
//  Trim helper
// ------------------------------------------------------------
std::string_view CSVLoader::trim(std::string_view sv) {
    while (!sv.empty() && std::isspace(sv.front())) sv.remove_prefix(1);
    while (!sv.empty() && std::isspace(sv.back()))  sv.remove_suffix(1);
    return sv;
}

// ------------------------------------------------------------
//  CSVLoader::load()
// ------------------------------------------------------------

Eigen::MatrixXd CSVLoader::load(
    const std::string& path,
    char delimiter,
    CSVLoadReport* report)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        if (logger_) logger_->error("CSVLoader: Could not open file: " + path);
        throw std::runtime_error("CSVLoader: Could not open file: " + path);
    }

    std::vector<std::vector<double>> rows;
    std::string line;
    size_t line_number = 0;
    size_t expected_cols = 0;

    if (report) {
        report->total_rows_read = 0;
        report->total_rows_loaded = 0;
        report->total_rows_dropped = 0;
        report->issues.clear();
        report->expected_columns = 0;
    }

    // ------------------------------------------------------------
    //  Read file line by line
    // ------------------------------------------------------------
    while (std::getline(file, line)) {
        line_number++;
        if (report) report->total_rows_read++;

        if (line.empty()) {
            drop_row(line_number, "Empty line", report, logger_);
            continue;
        }

        // ------------------------------------------------------------
        //  Split CSV line
        // ------------------------------------------------------------
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> values;

        while (std::getline(ss, cell, delimiter)) {
            values.push_back(std::string(trim(cell)));
        }

        // ------------------------------------------------------------
        //  Column consistency
        // ------------------------------------------------------------
        if (expected_cols == 0) {
            expected_cols = values.size();
            if (report) report->expected_columns = expected_cols;

            if (expected_cols == 0) {
                if (logger_) logger_->error("CSVLoader: First row contains no values.");
                throw std::runtime_error("CSVLoader: First row contains no values.");
            }
        }
        else if (values.size() != expected_cols) {

            if (logger_) {
                logger_->error(
                    "CSVLoader: Inconsistent number of columns in row " +
                    std::to_string(line_number)
                );
            }

            if (!strict_mode_) {
                drop_row(line_number, "Inconsistent number of columns", report, logger_);
                continue;
            }

            throw std::runtime_error(
                "CSVLoader: Inconsistent number of columns in row " +
                std::to_string(line_number)
            );
        }

        if (values.empty()) {
            drop_row(line_number, "Empty row", report, logger_);
            continue;
        }

        // ------------------------------------------------------------
        //  Convert to numeric
        // ------------------------------------------------------------
        std::vector<double> numeric_row;
        numeric_row.reserve(values.size());

        bool row_valid = true;

        for (const auto& v : values) {
            try {
                numeric_row.push_back(std::stod(v));
            }
            catch (...) {
                if (logger_) {
                    logger_->error(
                        "CSVLoader: Non-numeric value in row " +
                        std::to_string(line_number) + ": '" + v + "'"
                    );
                }
                drop_row(line_number, "Non-numeric value", report, logger_);
                row_valid = false;
                break;
            }
        }

        if (!row_valid) continue;

        // ------------------------------------------------------------
        //  Row is valid, store it
        // ------------------------------------------------------------
        rows.push_back(std::move(numeric_row));
        if (report) report->total_rows_loaded++;
    }

    // ------------------------------------------------------------
    //  Convert to Eigen::MatrixXd
    // ------------------------------------------------------------
    Eigen::MatrixXd mat(rows.size(), expected_cols);

    for (size_t i = 0; i < rows.size(); ++i) {
        for (size_t j = 0; j < expected_cols; ++j) {
            mat(i, j) = rows[i][j];
        }
    }

    if (logger_) {
        logger_->info(
            "CSVLoader: Loaded " +
            std::to_string(rows.size()) +
            " rows with " +
            std::to_string(expected_cols) +
            " columns."
        );
    }

    return mat;

}
