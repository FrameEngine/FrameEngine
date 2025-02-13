#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger {
private:
  Logger(const string &filename) {
    logFile.open(filename, ios::app);
    if (!logFile.is_open()) {
      cerr << "Error opening log file." << endl;
    }
  }

  ~Logger() { logFile.close(); }
  ofstream logFile;

  static Logger *instance;

  // Converts log level to a string for output
  string levelToString(LogLevel level) {
    switch (level) {
    case DEBUG:
      return "DEBUG";
    case INFO:
      return "INFO";
    case WARNING:
      return "WARNING";
    case ERROR:
      return "ERROR";
    case CRITICAL:
      return "CRITICAL";
    default:
      return "UNKNOWN";
    }
  }

public:
  static Logger &getInstance() {
    if (!instance) {
      instance = new Logger("main.log"); // TODO make it "flexible"
    }
    return *instance;
  }

  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

  void log(LogLevel level, const string &message) {
    time_t now = time(0);
    tm *timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    ostringstream logEntry;
    logEntry << "[" << timestamp << "] " << levelToString(level) << ": "
             << message << endl;

    cout << logEntry.str();

    if (logFile.is_open()) {
      logFile << logEntry.str();
      logFile.flush();
    }
  }
};

Logger *Logger::instance = nullptr;
