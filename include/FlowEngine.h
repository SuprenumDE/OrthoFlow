// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// OrthoFlow Orchestration
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 06.03.2026
// ------------------------------------------------------
// mkl.h-Hinweise für MS VS2026:
// Projekt-Eigenschaften: C++, Zusätzlich Include-Verzeichnisse:  C:\Program Files (x86)\Intel\oneAPI\mkl\latest\include
//                             Präprozessor-Definitionen:         EIGEN_USE_MKL_ALL
//                                                                ORTHOFLOW_USE_MKL
//                        Linker, Zusätzliche Abhängigkeiten:     mkl_intel_lp64.lib
//                                                                mkl_sequential.lib
//                                                                mkl_core.lib
//                                Zusätzliche Bib.-Verzeichnisse: C:\Program Files (x86)\Intel\oneAPI\mkl\latest\lib


#pragma once

#include <mkl.h>
#include <Eigen/Dense>
#include <functional>
#include <string>
#include <vector>
#include "FlowParams.h"
#include "csv_loader.h"
#include "logger.h"
#include "BaseScaler.h"
#include "StandardScaler.h"
#include "MinMaxScaler.h"
#include "RobustScaler.h"
#include "MaxAbsScaler.h"
#include "SVDEngine.h"
#include "SVDFactory.h"
#include "SVDStage.h"

namespace orthoflow {
    namespace flow {

        using orthoflow::decomposition::SVDEngine;
        
        enum class DataType {
            Unknown,
            Tabular,
            Image
            // später: Text, TimeSeries, Audio ...
        };

        using Matrix = Eigen::MatrixXd;

        struct FlowStep {
            std::string name;
            std::function<void(Matrix&)> op;
        };

        struct FlowResult {
            std::vector<std::string> stepNames;
            std::vector<Matrix> outputs;
        };

        struct FlowEngine {

            // --- Konstruktor: Logger ist Pflicht ---
            explicit FlowEngine(Logger* logger)
                : logger_(logger)
            {
            }

            FlowEngine() = delete;

            // --- Members ---
            std::vector<FlowStep> steps;
            Matrix initialInput = Matrix();
            Eigen::MatrixXd X_;
            Eigen::VectorXd y_;
            size_t label_column_ = 0;

            std::unique_ptr<BaseScaler> scaler_;
            std::unique_ptr<SVDEngine> svd_;


            FlowParams params_;
            CSVLoadReport csv_report_;
            Logger* logger_ = nullptr;

        private:
            // --- Identify data type ---
            DataType data_type_ = DataType::Unknown;

        public:
            DataType data_type() const { return data_type_; }
            void set_data_type(DataType dt) { data_type_ = dt; }

            // --- Parameter-Management ---
            void setParams(const FlowParams& p) { params_ = p; }
            const FlowParams& params() const { return params_; }

            // --- Pipeline ---
            void addStep(const FlowStep& step);
            FlowResult run() const;

            // --- Scaler ---
            void initialize_scaler();

            void initialize_svd();


        };

    } // namespace flow
} // namespace orthoflow
