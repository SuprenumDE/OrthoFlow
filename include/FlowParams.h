// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Define Parameters
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.08, 28.03.2026
// ------------------------------------------------------

#pragma once
#include <string>

enum class LabelColumnMode {
    First,
    Last,
    Index
};

struct FlowParams {
    std::string csv_file;                // The file to be loaded
    bool verbose;                        // Silence or chatter? True = chatter!
    char csv_delimiter;                  // Cell separator
    bool strict_mode;                    // Should execution be aborted in case of problems?
    std::string history_dir;             // Parameter storage, such, for example, scaler ...
    bool use_history;                    // Should the scaler history be used?
    std::string scaler_type;             // Scaler type
    std::string svd_variant;             // Which kind of SVD?

    // SVD reduction threshold (1.0 = no reduction)
    double svd_threshold = 1.0;

    // Plot mode (none | gnuplot)
    std::string svd_plot_mode = "none";

    LabelColumnMode label_mode = LabelColumnMode::First;
    size_t label_index = 0; // only relevant if mode = index

    //Parallelization
    size_t num_threads = 1;


    FlowParams(
        std::string csv_file_ = "data.csv",
        bool verbose_ = true,
        char csv_delimiter_ = ',',
        bool strict_mode_ = false,
        std::string history_dir_ = "",
        bool use_history_ = false,
        std::string scaler_type_ = "standard",
        std::string svd_variant_ = "jacobi",
        double svd_threshold_ = 1.0,
        std::string svd_plot_mode_ = "none"
    )
        : csv_file(std::move(csv_file_)),
        verbose(verbose_),
        csv_delimiter(csv_delimiter_),
        strict_mode(strict_mode_),
        history_dir(history_dir_),
        use_history(use_history_),
        scaler_type(scaler_type_),
        svd_variant(svd_variant_),
        svd_threshold(svd_threshold_),
        svd_plot_mode(std::move(svd_plot_mode_))
    {}
};

