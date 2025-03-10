#include "Logger.hpp"

namespace FrameEngine {

Logger *Logger::instance = nullptr;
std::mutex Logger::logMutex;

} // namespace FrameEngine
