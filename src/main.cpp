// --------------------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/Dense_NN
// Lizenz: MIT (also for all self-developed included h and cpp files)
// Version 01.00, 03.04.2026
// Eigen-Version: 3.4.0
// cnpy: Copyright (C) 2011  Carl Rogers
// zlib: Copyright (C) 1995-2024 Jean-loup Gailly and Mark Adler
// mkl.h: MKL integration via Intel oneAPI, see also FlowEngine.h
// C-Version: ISO-Standard C++20
// --------------------------------------------------------------------

#include <iostream>
#include <filesystem>
#include "FlowEngine.h"
#include "csv_loader.h"
#include "logger.h"
#include "FlowParams.h"
#include "ParamLoader.h"
#include "split.h"
#include "CSVWriter.h"
#include "PlotUtils.h"


using namespace orthoflow::flow;
std::unique_ptr<SVDEngine> svd_ = nullptr;


int main(int argc, char** argv) {

    try {
        // Load parameters
        FlowParams params = ParamLoader::from_cli(argc, argv);

        // Thread correction (avoid hyper-threading)
        size_t hw = std::thread::hardware_concurrency();
        if (params.num_threads > hw) {
            std::cerr << "[Warning] Requested " << params.num_threads
                << " threads, but only " << hw
                << " hardware threads available. Clamping.\n";
            params.num_threads = hw;
        }

        // Parallelization
        Eigen::initParallel();
        Eigen::setNbThreads(params.num_threads);

        // ---------------- Logger ------------------------
        // Dynamischer Log-Dateiname
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        std::tm tm{};

#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

        // Log-Verzeichnis sicherstellen
        std::filesystem::create_directories(params.history_dir + "/logs");

        std::string log_path =
            params.history_dir + "/logs/" + ss.str() + "_OrthoFlowPipeline.log";

        Logger logger(log_path);

        // ---------------- Logger End ---------------------

        logger.info("Using " + std::to_string(params.num_threads) + " thread(s).");

        // Engine initialisieren
        FlowEngine engine(&logger);
        engine.setParams(params);
        engine.initialize_scaler();
        engine.initialize_svd();



        // Load CSV
        engine.addStep({
            "Load CSV",
            [&](Matrix& m) {
                CSVLoader loader(&logger, engine.params().strict_mode);
                engine.csv_report_ = CSVLoadReport{};

                m = loader.load(
                    engine.params().csv_file,
                    engine.params().csv_delimiter,
                    &engine.csv_report_
                );
            }
            });

        // Detect Data Type
        engine.addStep({
           "Detect Data Type",
           [&](Matrix& m) {
               if (m.rows() > 1 && m.cols() > 1) {
                   engine.set_data_type(DataType::Tabular);
                   if (engine.params().verbose)
                       std::cout << "[Info] Data type detected: Tabular\n";
               }
            else {
            engine.set_data_type(DataType::Unknown);
                if (engine.params().verbose)
                std::cout << "[Info] Data type detected: Unknown\n";
                 }
               }
            });

        // Split Features and Labels
        engine.addStep({
            "Split Features/Label",
            [&](Matrix& m) {

                size_t label_col = 0;

                Splitter splitter;
                auto split = splitter.split(m, label_col);

                engine.X_ = split.X;
                engine.y_ = split.y;
                engine.label_column_ = label_col;

                if (engine.params().verbose) {
                    std::cout << "[Info] Split: Using column " << label_col << " as label column.\n";
                    std::cout << "[Info] Feature matrix shape: "
                              << engine.X_.rows() << " x " << engine.X_.cols() << "\n";
                    std::cout << "[Info] Label vector length: "
                              << engine.y_.size() << "\n";
                }

                // Pipeline-Matrix = Feature-Matrix
                m = engine.X_;
            }
            });

        // Scale Features
        engine.addStep({
           "Scale Features",
           [&](Matrix& m) {

               std::filesystem::create_directories(params.history_dir);
               std::string scaler_path = params.history_dir + "/scaler.json";
               bool scaler_exists = std::filesystem::exists(scaler_path);

               if (params.use_history && scaler_exists) {
                   if (params.verbose)
                       std::cout << "[Info] Loading existing scaler from: " << scaler_path << "\n";
                   engine.scaler_->load(scaler_path);
               }
               else if (params.use_history && !scaler_exists) {
                   if (params.verbose)
                       std::cout << "[Info] No existing scaler found. Fitting new scaler.\n";
                   engine.scaler_->fit(m);
                   engine.scaler_->save(scaler_path);
               }
               else {
                   if (params.verbose)
                       std::cout << "[Info] History disabled. Fitting new scaler.\n";
                   engine.scaler_->fit(m);
                   engine.scaler_->save(scaler_path);
               }

               // Transformation auf Pipeline-Matrix
               m = engine.scaler_->transform(m);

               // Optional: Engine-Kopie aktualisieren
               engine.X_ = m;

            }
            });

        // Calculating SVD
        engine.addStep({
            "Compute SVD",
            [&](Matrix& m) {

                std::filesystem::create_directories(params.history_dir);
                std::string svd_path = params.history_dir + "/svd.json";
                bool svd_exists = std::filesystem::exists(svd_path);

                if (params.use_history && svd_exists) {
                    if (params.verbose)
                        std::cout << "[Info] Loading existing SVD from: " << svd_path << "\n";
                    engine.svd_->load(svd_path);
                }
                else if (params.use_history && !svd_exists) {
                    if (params.verbose)
                        std::cout << "[Info] No existing SVD found. Computing new SVD.\n";

                    if (engine.params().verbose)
                        std::cout << "[Debug] X before SVD: "
                                  << m.rows() << " x " << m.cols() << "\n";

                    engine.svd_->compute(m);
                    engine.svd_->save(svd_path);
                }
                else {
                    if (params.verbose)
                        std::cout << "[Info] History disabled. Computing new SVD.\n";

                    engine.svd_->compute(m);
                    engine.svd_->save(svd_path);
                }

                // Pipeline-Matrix = Projektion
                m = engine.svd_->result().projection;

                // Optional: Engine-Kopie
                engine.X_ = m;
            }
            });


        // CSV Writer for Y & SVD Projection
        engine.addStep({
           "Export y+Projection CSV",
           [&](Matrix& m) {

                std::string out_csv = params.history_dir + "/dense_input.csv";
                CSVWriter::write_yX(out_csv, engine.y_, m);


               std::cout << "[Info] Saved combined y+X CSV to dense_input.csv\n";

            }
            });


        // Reporting
        engine.addStep({
            "CSV Report",
            [&](Matrix&) {

                if (!engine.params().verbose)
                    return;

                const auto& report = engine.csv_report_;

                std::cout << "\n=== CSV Load Report ===\n";
                std::cout << "Loaded file : " << engine.params().csv_file << "\n";
                std::cout << "Rows read   : " << report.total_rows_read << "\n";
                std::cout << "Rows loaded : " << report.total_rows_loaded << "\n";
                std::cout << "Rows dropped: " << report.total_rows_dropped << "\n";
                std::cout << "Columns     : " << report.expected_columns << "\n";

                if (!report.issues.empty()) {
                    std::cout << "\nIssues:\n";
                    for (const auto& issue : report.issues) {
                        std::cout << "  Line " << issue.line
                                  << ": " << issue.message;
                        if (!issue.offending_value.empty())
                            std::cout << " ('" << issue.offending_value << "')";
                        std::cout << "\n";
                    }
                }
                std::cout << "Label column: " << engine.label_column_ << "\n";
                std::cout << "========================\n\n";
            }
            });


        // SVD Scree Plot (optional)
        if (engine.params().svd_plot_mode == "gnuplot") {
            engine.addStep({
                "SVD Scree Plot",
                [&](Matrix&) {
                    const auto& svd = engine.svd_;

                    // Zielpfad im History-Ordner
                    std::string out = engine.params().history_dir + "/scree.png";

                    plot_scree(svd->result().singular_values_full, out, svd->k_selected(), engine.params().svd_threshold, svd->variance_explained() );

                    std::cout << "[Plot] Scree-Plot gespeichert unter: " << out << "\n";
                }
                });
        }



        engine.addStep({
            "Print shapes",
            [&](Matrix&) {
                std::cout << "Original matrix shape: "
                          << engine.csv_report_.total_rows_loaded << " x "
                          << engine.csv_report_.expected_columns << "\n";

                std::cout << "Feature matrix shape: "
                          << engine.X_.rows() << " x " << engine.X_.cols() << "\n";

                std::cout << "Label vector length: "
                          << engine.y_.size() << "\n";
            }
            });


        // SVD Reduction Report
        engine.addStep({
            "SVD Report",
            [&](Matrix&) {

                if (!engine.params().verbose)
                    return;

                const auto& svd = engine.svd_;

                std::cout << "\n=== SVD Reduction Report ===\n";

                std::cout << "SVD variant        : " << engine.params().svd_variant << "\n";
                std::cout << "Threshold          : " << engine.params().svd_threshold << "\n";

                std::cout << "Selected k         : " << svd->k_selected() << "\n";
                std::cout << "Variance explained : "
                          << svd->variance_explained() * 100.0 << " %\n";

                std::cout << "Original dims      : "
                          << svd->result().Vt.cols() << "\n";

                std::cout << "Reduced dims       : "
                          << svd->result().Vt.rows() << "\n";

                std::cout << "Reduction factor   : "
                          << static_cast<double>(svd->result().Vt.cols()) /
                             static_cast<double>(svd->result().Vt.rows())
                          << "x\n";

                std::cout << "=============================\n\n";
            }
            });


        // Pipeline starten
        FlowResult result = engine.run();
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
    }

    return 0;
}

