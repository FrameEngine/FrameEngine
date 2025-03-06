/**
 * @file Logger.hpp
 * @brief Provides logging functionality.
 *
 * The logger is implemented as a thread-safe singleton, writing logs to both
 * the console and a log file.
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <fmt/printf.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

/**
 * @enum LogLevel
 * @brief Defines the logging levels.
 *
 * This enum lists the possible severity levels for log messages.
 */
enum class LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger {
private:
  std::ofstream logFile;      ///< The file stream used for logging to a file.
  static Logger *instance;    ///< Singleton instance of Logger.
  static std::mutex logMutex; ///< Mutex to protect logging operations.

  /**
   * @brief Private constructor for the Logger singleton.
   *
   * Initializes the logger by setting the default log file.
   */
  Logger() { setLogFile("main.log"); }

  /**
   * @brief Converts a log level to a string.
   *
   * @param level The log level.
   * @return A string representing the log level.
   */
  static std::string levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::CRITICAL:
      return "CRITICAL";
    default:
      return "UNKNOWN";
    }
  }

public:
  /**
   * @brief Returns the singleton instance of Logger.
   *
   * This method guarantees that only one Logger exists.
   *
   * @return A reference to the Logger instance.
   */
  static Logger &getInstance() {
    static Logger instance; // Thread-safe singleton
    return instance;
  }

  /**
   * @brief Sets the log file.
   *
   * Closes the current log file (if open) and opens a new file for appending
   * logs.
   *
   * @param filename The name of the log file to use.
   */
  void setLogFile(const std::string &filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
      logFile.close();
    }

    logFile.open(filename, std::ios::app);
    if (!logFile) {
      std::cerr << "Error opening log file: " << filename << std::endl;
    }
  }

  /**
   * @brief Logs a message with the specified log level.
   *
   * This templated function formats the message using the given format string
   * and arguments, prefixes it with the current timestamp and log level,
   * and writes the log to both the console and the log file.
   *
   * @tparam Args The types of the additional arguments.
   * @param level The log level for the message.
   * @param format The format string.
   * @param args The values to be formatted into the message.
   */
  template <typename... Args>
  void log(LogLevel level, const char *format, Args &&...args) {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string formattedMessage =
        fmt::sprintf(format, std::forward<Args>(args)...);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm timeinfo = *std::localtime(&now_c);

    std::ostringstream logEntry;
    logEntry << "[" << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "] "
             << levelToString(level) << ": " << formattedMessage << "\n";

    std::cout << logEntry.str();

    if (logFile.is_open()) {
      logFile << logEntry.str();
      logFile.flush();
    }
  }
};

/**
 * @brief Macro for simplified logging.
 *
 * This macro allows to log messages without explicitly calling getInstance().
 * Usage: LOG(DEBUG, "Value: %d", value);
 */
#define LOG(level, fmt, ...)                                                   \
  Logger::getInstance().log(LogLevel::level, fmt, ##__VA_ARGS__)

#endif // LOGGER_HPP
