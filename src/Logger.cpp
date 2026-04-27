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


#include "Logger.h"
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filepath)
    : file_(filepath, std::ios::out | std::ios::app)
{
    if (!file_.is_open()) {
        throw std::runtime_error("Logger: Could not open log file: " + filepath);
    }
}

void Logger::info(const std::string& msg) {
    write(LogLevel::Info, msg);
}

void Logger::warning(const std::string& msg) {
    write(LogLevel::Warning, msg);
}

void Logger::error(const std::string& msg) {
    write(LogLevel::Error, msg);
}

void Logger::write(LogLevel level, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mutex_);  // Thread!

    // Timestamp
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    tm = *std::localtime(&t);   // Linux, macOS, Raspberry Pi
#endif


    file_ << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " ";

    switch (level) {
    case LogLevel::Info:    file_ << "[INFO] "; break;
    case LogLevel::Warning: file_ << "[WARN] "; break;
    case LogLevel::Error:   file_ << "[ERROR] "; break;
    }

    file_ << msg << "\n";
}
