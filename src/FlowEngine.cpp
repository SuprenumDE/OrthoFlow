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
// Version 0.0.02, 24.03.2026
// ------------------------------------------------------


#include "FlowEngine.h"
#include <iostream>

namespace orthoflow {
    namespace flow {


        // ---------------------------------------------------------
        // Step hinzufügen
        // ---------------------------------------------------------
        void FlowEngine::addStep(const FlowStep& step) {
            steps.push_back(step);

            if (logger_) {
                logger_->info("FlowEngine: Added step: " + step.name);
            }
        }

        // ---------------------------------------------------------
        // Pipeline ausführen
        // ---------------------------------------------------------
        FlowResult FlowEngine::run() const {
            FlowResult result;

            if (logger_) {
                logger_->info("FlowEngine: Starting pipeline with "
                    + std::to_string(steps.size()) + " steps.");
            }

            Matrix current = initialInput;

            for (const auto& step : steps) {

                if (logger_) {
                    logger_->info("FlowEngine: Running step: " + step.name);
                }

                Matrix tmp = current;      // Kopie für die Operation
                step.op(tmp);              // Operation ausführen

                result.stepNames.push_back(step.name);
                result.outputs.push_back(tmp);

                current = tmp;             // Ergebnis wird neuer Input
            }

            if (logger_) {
                logger_->info("FlowEngine: Pipeline finished.");
            }

            return result;
        }

        // ---------------------------------------------------------
        // Scaler initalisieren
        // ---------------------------------------------------------
        void FlowEngine::initialize_scaler() {
            if (params_.scaler_type == "standard") {
                scaler_ = std::make_unique<StandardScaler>();
            }
            else if (params_.scaler_type == "minmax") {
                scaler_ = std::make_unique<MinMaxScaler>();
            }
            else if (params_.scaler_type == "robust") {
                scaler_ = std::make_unique<RobustScaler>();
            }
            else if (params_.scaler_type == "maxabs") {
                scaler_ = std::make_unique<MaxAbsScaler>();
            }
            else {
                throw std::runtime_error("Unknown scaler type: " + params_.scaler_type);
            }
        }

        // ---------------------------------------------------------
        // SVD initalisieren
        // ---------------------------------------------------------
        void FlowEngine::initialize_svd() {
            using namespace orthoflow::decomposition;

            // Factory erzeugen
            auto factory = std::make_unique<SVDFactory>();

            // Stage erzeugen (Factory + Variante)
            auto stage = std::make_unique<SVDStage>(
                std::move(factory),
                params_.svd_variant   // z. B. "jacobi" oder "bdc"
            );

            // Engine erzeugen (nur Threshold!)
            svd_ = std::make_unique<SVDEngine>(
                std::move(stage),
                params_.svd_threshold
            );

            if (logger_) {
                logger_->info(
                    "FlowEngine: Initialized SVD engine with variant: "
                    + params_.svd_variant +
                    ", threshold: " + std::to_string(params_.svd_threshold)
                );
            }
        }






    } // namespace flow
} // namespace orthoflow

