#include "log.hpp"

#include <filesystem>

Log::Log(const std::shared_ptr<Config> &config)
    : config_(config), level_(Level::INFO) {

    if (config_->runMode() == RunMode::server)
        mode_ = std::string("SERVER");
    else if (config_->runMode() == RunMode::agent)
        mode_ = std::string("AGENT");

    try {
        const std::filesystem::path logPath(config_->log().file);
        if (logPath.has_parent_path()) {
            std::filesystem::create_directories(logPath.parent_path());
        }
    } catch (const std::exception &e) {
        std::cerr << "Error creating log directory for: " << config_->log().file
                  << ". " << e.what() << "\n";
    }

    std::ofstream logFile(config_->log().file, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file: " << config_->log().file
                  << ". Make sure the directory and file exist.\n";
    }

    const auto &logLevel = config_->log().level;

    if (logLevel == "INFO") {
        level_ = Level::INFO;
    } else if (logLevel == "TRACE") {
        level_ = Level::TRACE;
    } else if (logLevel == "DEBUG") {
        level_ = Level::DEBUG;
    } else {
        std::cerr << "Invalid log level: " << logLevel
                  << ". It should be one of [INFO|TRACE|DEBUG].\n";
    }
}

Log::Log(const std::shared_ptr<Log> &log)
    : config_(log->config_), level_(log->level_) {}

Log::~Log() {}

void Log::write(const std::string &message, Level level) const {
    std::lock_guard<std::mutex> lock(logMutex_);

    if (level <= level_ || level == Level::ERROR) {

        try {
            const std::filesystem::path logPath(config_->log().file);
            if (logPath.has_parent_path()) {
                std::filesystem::create_directories(logPath.parent_path());
            }
        } catch (const std::exception &e) {
            std::cerr << "Error creating log directory for: " << config_->log().file
                      << ". " << e.what() << "\n";
        }

        std::ofstream logFile(config_->log().file, std::ios::out | std::ios::app);

        if (logFile.is_open()) {


            auto now = std::time(nullptr);
            auto localTime = *std::localtime(&now);

            std::ostringstream timestampStream;
            timestampStream << std::put_time(&localTime, "%Y-%m-%d_%H:%M:%S");
            std::string timestamp = timestampStream.str();

            std::string line = timestamp + " [" + mode_ + "]" + " [" +
                               levelToString(level) + "] " + message + "\n";

            logFile << line;
            std::cout << line;

        } else {
            std::cerr << "Error opening log file: " << config_->log().file
                      << ". Make sure the directory and file exist.\n";
        }
    }
}

std::string Log::levelToString(Level level) {
    switch (level) {
        case Level::INFO:
            return "INFO";
        case Level::TRACE:
            return "TRACE";
        case Level::ERROR:
            return "ERROR";
        case Level::DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
    }
}