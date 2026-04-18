#pragma once

#include <condition_variable>
#include <format>
#include <fstream>
#include <queue>
#include <string>
#include <thread>
#include <stdarg.h>

/**
 * @brief Levels of logging
 * @version 0.1.0
 */
enum class WriteX_Level {
  DEBUG, INFO, WARNING, ERROR, FATAL
};

/**
 * @brief Class for logging
 * @version 0.0.1
 */
class WriteX {
private:
  std::thread bgthread;
  std::condition_variable cv;
  bool stop_flag {false};

  std::mutex mtx;
  std::ofstream log_file;
  std::queue<std::string> msg_queue;

  std::string logger_name;
  std::string fmt;

  void enq_msg(std::string&);
  void loop();
public:
  WriteX(const std::string&, const std::string&);
  ~WriteX();
  std::string levelToString(WriteX_Level&);
  void setFormat(std::string&);
  std::string format(WriteX_Level, const std::string&, const char*, const char*, int);

  template<typename ...Args>
  std::string format_msg(const std::string& msg, Args... args) {
    return std::vformat(msg, std::make_format_args(args...));
  }

  template<typename ...Args>
  void log(const WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line, Args... args) {
    std::string str = format(lvl, format_msg(msg, args...), file, func, line);
    enq_msg(str);
  }
};

#ifndef LOG_DEBUG
#define LOG_DEBUG(logger, message, ...) \
logger.log(WriteX_Level::DEBUG, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_DEBUG

#ifndef LOG_INFO
#define LOG_INFO(logger, message, ...) \
logger.log(WriteX_Level::INFO, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_INFO

#ifndef LOG_WARNING
#define LOG_WARNING(logger, message, ...) \
logger.log(WriteX_Level::WARNING, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_WARNING

#ifndef LOG_ERROR
#define LOG_ERROR(logger, message, ...) \
logger.log(WriteX_Level::ERROR, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_ERROR


#ifndef LOG_FATAL
#define LOG_FATAL(logger, message, ...) \
logger.log(WriteX_Level::FATAL, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_FATAL