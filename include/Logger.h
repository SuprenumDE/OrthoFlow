// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// OrthoFlow Logbook data download/check
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 25.01.2026
// ------------------------------------------------------


#pragma once
#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
public:
    explicit Logger(const std::string& filepath);

    void info(const std::string& msg);
    void warning(const std::string& msg);
    void error(const std::string& msg);

private:
    void write(LogLevel level, const std::string& msg);

    std::ofstream file_;    // RAII-based, clean termination
    std::mutex mutex_;      // Thread-safe
};

