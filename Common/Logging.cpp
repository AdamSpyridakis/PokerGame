#include "Logging.hpp"

enum LogLevel {
    Debug,
    Info,
    Error
};

static const LogLevel g_logLevel = Info;

void logError(std::string tag, std::string log) {
    switch (g_logLevel) {
        case Debug:
        case Info:
        case Error:
            std::cout << "\033[31m[ERROR]\033[0m " << tag << ": " << log << std::endl;
            return;
        default:
            return;
    }
}

void logDebug(std::string tag, std::string log) {
    switch (g_logLevel) {
        case Debug:
            std::cout << "\033[34m[DEBUG]\033[0m " << tag << ": " << log << std::endl;
            return;
        default:
            return;
    }
}

void logInfo(std::string tag, std::string log) {
    switch (g_logLevel) {
        case Debug:
        case Info:
            std::cout << "\033[32m[INFO]\033[0m " << tag << ": " << log << std::endl;
            return;
        default:
            return;
    }
}

void logEssential(std::string log) {
    std::cout << "\033[33m[ESSSENTIAL]\033[0m " << ": " << log << std::endl;
}