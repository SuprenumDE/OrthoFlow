// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Load Parameters
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.04, 28.03.2026
// ------------------------------------------------------

#include "ParamLoader.h"
#include <cxxopts.hpp>
#include <iostream>
#include <thread>


// Utility Function:
bool to_bool(const std::string& s) {
    if (s == "true" || s == "1" || s == "yes") return true;
    if (s == "false" || s == "0" || s == "no") return false;
    throw std::runtime_error("Invalid boolean: " + s);
}


FlowParams ParamLoader::from_cli(int argc, char** argv) {
    cxxopts::Options options("OrthoFlow", "SVD-based feature engineering pipeline");

    options.add_options()
        ("f,file", "CSV file to load", cxxopts::value<std::string>()->default_value("data.csv"))
        ("d,delimiter", "CSV delimiter", cxxopts::value<char>()->default_value(","))
        ("s,strict", "Strict mode (abort on errors)", cxxopts::value<bool>()->default_value("false"))
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("true"))
        ("l,label", "Label column: first | last | index:<n>", cxxopts::value<std::string>()->default_value("first"))
        ("scaler", "Scaler type: standard | minmax | robust | maxabs", cxxopts::value<std::string>()->default_value("standard"))
        ("history-dir", "Directory for scaler/model history",cxxopts::value<std::string>()->default_value(""))
        ("use-history", "Load existing scaler/model history if available",cxxopts::value<std::string>()->default_value("false"))
        ("svd-variant", "SVD variant: jacobi | bdc | mkl",cxxopts::value<std::string>()->default_value("jacobi"))
        ("svd-threshold", "Threshold for automatic SVD reduction (0 < t <= 1)",cxxopts::value<double>()->default_value("1.0"))
        ("svd-plot-mode", "Plot mode: none | gnuplot",cxxopts::value<std::string>()->default_value("none"))
        ("threads", "Number of threads: auto | 1 | N",cxxopts::value<std::string>()->default_value("auto"))
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        std::exit(0);
    }

    // Create FlowParams
    FlowParams params{
        result["file"].as<std::string>(),
        result["verbose"].as<bool>(),
        result["delimiter"].as<char>(),
        result["strict"].as<bool>()
    };

    // Label column parsing
    std::string label_arg = result["label"].as<std::string>();


    if (label_arg == "first") {
        params.label_mode = LabelColumnMode::First;
    }
    else if (label_arg == "last") {
        params.label_mode = LabelColumnMode::Last;
    }
    else if (label_arg.rfind("index:", 0) == 0) {
        params.label_mode = LabelColumnMode::Index;
        params.label_index = std::stoul(label_arg.substr(6));
    }
    else {
        std::cerr << "[Warning] Unknown label argument '" << label_arg
            << "'. Falling back to 'first'.\n";
        params.label_mode = LabelColumnMode::First;
    }

    // History directory
    params.history_dir = result["history-dir"].as<std::string>();
   
    // If empty, set default
    if (params.history_dir.empty()) {
        params.history_dir = "./orthoflow_output";
    }

    // Ensure directory exists
    std::filesystem::create_directories(params.history_dir);

    // Use history?
    params.use_history = to_bool(result["use-history"].as<std::string>());


    // Assign scaler
    params.scaler_type = result["scaler"].as<std::string>();

    // Assign SVD variant
    params.svd_variant = result["svd-variant"].as<std::string>();

    // Validate SVD variant
    if (params.svd_variant != "jacobi" &&
        params.svd_variant != "bdc" &&
        params.svd_variant != "mkl")
    {
        throw std::runtime_error("svd-variant must be one of: jacobi | bdc | mkl");
    }

#ifndef ORTHOFLOW_USE_MKL
    if (params.svd_variant == "mkl") {
        throw std::runtime_error(
            "svd-variant=mkl requested, but ORTHOFLOW_USE_MKL is not enabled."
        );
    }
#endif


    // Threshold value cumulative variance to determine the reduction factor
    params.svd_threshold = result["svd-threshold"].as<double>();
    if (params.svd_threshold <= 0.0 || params.svd_threshold > 1.0) {
        throw std::runtime_error("svd-threshold must be in (0, 1].");
    }

    // Should a GNUPlot graph be displayed, for example, to show the cumulative variance
    // for determining the reduction factor?
    params.svd_plot_mode = result["svd-plot-mode"].as<std::string>();
    if (params.svd_plot_mode != "none" && params.svd_plot_mode != "gnuplot") {
        throw std::runtime_error("svd-plot-mode must be 'none' or 'gnuplot'.");
    }


    // Parallelization
    std::string threads_arg = result["threads"].as<std::string>();

    if (threads_arg == "auto") {
        params.num_threads = std::thread::hardware_concurrency();
    }
    else {
        params.num_threads = std::stoul(threads_arg);

        if (params.num_threads == 0) {
            throw std::runtime_error("threads must be >= 1 or 'auto'.");
        }
    }

    return params;
}


