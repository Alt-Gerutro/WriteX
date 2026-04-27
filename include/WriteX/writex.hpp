#pragma once

#include <condition_variable>
#include <format>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <ostream>

/**
 * @note All macros for filtering
 */

#define WRITEX_ALL_LEVELS static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define WRITEX_TO_INFO static_cast<short> \
  (WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define WRITEX_TO_WARNING static_cast<short> \
  (WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define WRITEX_TO_ERROR static_cast<short> \
  (WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define WRITEX_TO_ERROR_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR)

#define WRITEX_TO_WARNING_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING)

#define WRITEX_TO_INFO_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO)

/**
 * @brief Levels of logging
 */
enum class WriteX_Level {
  DEBUG = (1 << 0), 
  INFO = (1 << 1), 
  WARNING = (1 << 2), 
  ERROR = (1 << 3), 
  FATAL = (1 << 4)
};

/**
 * @brief Add operator bit-or for WriteX_Level enum class
 * 
 * @param a Log level left
 * @param b Log level right
 * @return WriteX_Level level
 */
const WriteX_Level operator|(WriteX_Level a, WriteX_Level b);

/**
 * @brief Class for logging
 */
class WriteX {
private:
  std::thread bgthread;
  std::condition_variable cv;
  bool stop_flag {false};

  std::mutex mtx;
  std::shared_ptr<std::ostream> ostream;
  std::queue<std::string> msg_queue;

  std::string logger_name;
  std::string fmt;
  short filter_level;

  bool add_newline;

  void enq_msg(std::string&);
  void loop();
public:
  class Builder {
    friend WriteX;
    std::string nm;
    std::string fmt {"[%N] [%F %f:%l] [%L] %M"};
    short ftr {WRITEX_ALL_LEVELS};
    bool nl {true};
    std::shared_ptr<std::ostream> output;

    public:
      Builder(const std::string name);
      Builder& format(const std::string format);
      Builder& filter(short format);
      Builder& newline(bool newline);
      Builder& output_stream(std::shared_ptr<std::ostream> stream);
      
      std::shared_ptr<WriteX> build();
  };

  explicit WriteX(Builder& builder);
  ~WriteX();

  /**
   * @brief Transform level to std::string
   * 
   * @param level Log level
   * @return std::string Stringificated level
   */
  std::string levelToString(const WriteX_Level& level) const;

  /**
   * @brief Switching new line flag
   * 
   * @note true -> false, false -> true.
   * @note By default: true
   */
  void switchNewLine();

  /**
   * @brief Set the Filter of logs
   * 
   * @param filter Integer for filter
   *
   * @note See how to works filter integer in README.md
   */
  void setFilter(short filter);

  /**
   * @brief Get the filter level integer
   * 
   * @return short filter integer
   */
  short getFilter();

  /**
   * @brief Set the format string
   * 
   * @param format_string Format string
   */
  void setFormat(std::string format_string);

  /**
   * @brief Get the Format string
   * 
   * @return std::string Format string
   */
  std::string getFormat();

  /**
   * @brief Format string for log
   * 
   * @param lvl Log level
   * @param msg Message
   * @param file File where log used
   * @param func Function where log used
   * @param line Line of code where log used
   * @return std::string Formatted string
   */
  std::string format(WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line);

  /**
   * @brief Format user message
   * 
   * @tparam Args Arguments type
   * @param msg Message (and format string)
   * @param args Arguments
   * @return std::string Formatted string
   */
  template<typename ...Args>
  std::string format_msg(const std::string& msg, Args&&... args) {
    try {
      return std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));;
    }
    catch (std::format_error) {
      return std::vformat("CANNOT FORMAT STRING: {}", std::make_format_args(msg));
    }
  }

  /**
   * @brief Log message
   * 
   * @tparam Args Arguments type
   * @param lvl Log level
   * @param msg Message
   * @param file File where log used
   * @param func Function where log used
   * @param line Line of code where log used
   * @param args Arguments for message
   */
  template<typename ...Args>
  void log(const WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line, Args&&... args) {
    try {
      short cur_filter;
      {
        std::lock_guard<std::mutex> lock(mtx);
        cur_filter = filter_level;
      }
      if (static_cast<short>(lvl) & cur_filter) {
        std::string str = format(lvl, format_msg(msg, std::forward<Args>(args)...), file, func, line);
        enq_msg(str);
      }
    }
    catch (const std::format_error) {
      this->log(WriteX_Level::FATAL, "CANNOT FORMAT MESSAGE: {}", file, func, line, msg);
    } 
  }

  /**
   * @brief Wait for log output
   */
  void flush();
};

// ********** MACROS **********

/**
 * @note All macros for logging
 */

#ifndef WRITEX_LOG_DEBUG
#define WRITEX_LOG_DEBUG(logger, message, ...) \
logger->log(WriteX_Level::DEBUG, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_DEBUG

#ifndef WRITEX_LOG_INFO
#define WRITEX_LOG_INFO(logger, message, ...) \
logger->log(WriteX_Level::INFO, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // LOG_INFO

#ifndef WRITEX_LOG_WARNING
#define WRITEX_LOG_WARNING(logger, message, ...) \
logger->log(WriteX_Level::WARNING, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // WRITEX_LOG_WARNING

#ifndef WRITEX_LOG_ERROR
#define WRITEX_LOG_ERROR(logger, message, ...) \
logger->log(WriteX_Level::ERROR, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // WRITEX_LOG_ERROR

#ifndef WRITEX_LOG_FATAL
#define WRITEX_LOG_FATAL(logger, message, ...) \
logger->log(WriteX_Level::FATAL, message, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif // WRITEX_LOG_FATAL
