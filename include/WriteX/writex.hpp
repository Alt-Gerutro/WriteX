#pragma once

#include <condition_variable>
#include <format>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <stdarg.h>

/**
 * @brief Levels of logging
 * @version 0.1.0
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
 * @version 0.2.0
 */
class WriteX {
private:
  std::thread bgthread;
  std::condition_variable cv;
  bool stop_flag {false};

  std::mutex mtx;
  std::ostream& ostream;
  std::queue<std::string> msg_queue;

  std::string logger_name;
  std::string fmt;
  short filter_level;

  bool add_newline {true};

  void enq_msg(std::string&);
  void loop();
public:
  /**
   * @brief Construct a new WriteX object
   * 
   * @param name Name of logger
   * @param _ostream Stream where to write logs
   *
   * @version 0.2.0
   */
  explicit WriteX(const std::string& name, std::ostream& _ostream);

  /**
   * @brief Construct a new WriteX object
   * 
   * @param name Name of logger
   * @param _ostream Stream where to write logs
   * @param format Format string. See format documentation.
   *
   * @version 0.2.0
   */
  explicit WriteX(const std::string& name, std::ostream& _ostream, const std::string& format);

  /**
   * @brief Construct a new WriteX object
   * 
   * @param name Name of logger
   * @param _ostream Stream where to write logs
   * @param filter Filter integer. See iltering documentation
   *
   * @version 0.2.0
   */
  explicit WriteX(const std::string& name, std::ostream& _ostream, short filter);

  /**
   * @brief Construct a new WriteX object
   * 
   * @param name Name of logger
   * @param _ostream Stream where to write logs
   * @param format Format string. See format documentation
   * @param filter Filter integer. See iltering documentation
   *
   * @version 0.2.0
   */
  explicit WriteX(const std::string& name, std::ostream& _ostream, const std::string& format, short filter);
  ~WriteX();

  /**
   * @brief Transform level to std::string
   * 
   * @param level Log level
   * @return std::string Stringificated level
   *
   * @version 0.1.0
   */
  std::string levelToString(const WriteX_Level& level) const;

  /**
   * @brief Switching new line flag
   * 
   * @note true -> false, false -> true.
   * @note By default: true
   * @version 0.2.0
   */
  void switchNewLine();

  /**
   * @brief Set the Filter of logs
   * 
   * @param filter Integer for filter
   *
   * @note See how to works filter integer in README.md
   * 
   * @version 0.1.2
   */
  void setFilter(short filter);

  /**
   * @brief Get the filter level integer
   * 
   * @return short filter integer
   *
   * @version 0.1.2
   */
  short getFilretInt();

  /**
   * @brief Set the format string
   * 
   * @param format_string Format string
   * 
   * @version 0.1.0
   */
  void setFormat(std::string format_string);

  /**
   * @brief Get the Format string
   * 
   * @return std::string Format string
   *
   * @version 0.1.2
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
   *
   * @version 0.1.0
   */
  std::string format(WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line);

  /**
   * @brief Format user message
   * 
   * @tparam Args Arguments type
   * @param msg Message (and format string)
   * @param args Arguments
   * @return std::string Formatted string
   *
   * @version 0.1.0
   */
  template<typename ...Args>
  std::string format_msg(const std::string& msg, Args... args) {
    return std::vformat(msg, std::make_format_args(args...));
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
   *
   * @version 0.1.0
   */
  template<typename ...Args>
  void log(const WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line, Args... args) {
    short cur_filter;
    {
      std::lock_guard<std::mutex> lock(mtx);
      cur_filter = filter_level;
    }
    if (static_cast<short>(lvl) & cur_filter) {
      std::string str = format(lvl, format_msg(msg, args...), file, func, line);
      enq_msg(str);
    }
  }

  /**
   * @brief Wait for log output
   * 
   * @version 0.2.0
   */
  void flush();
};

// ********** MACROS **********

/**
 * @note All macros for logging added in
 * @version 0.1.0
 */

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

/**
 * @note All macros for filtering added in
 * @version 0.1.2
 */

#define ALL_LEVELS static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define TO_INFO static_cast<short> \
  (WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define TO_WARNING static_cast<short> \
  (WriteX_Level::WARNING | \
  WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define TO_ERROR static_cast<short> \
  (WriteX_Level::ERROR | \
  WriteX_Level::FATAL)

#define TO_ERROR_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING | \
  WriteX_Level::ERROR)

#define TO_WARNING_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO | \
  WriteX_Level::WARNING)

#define TO_INFO_ASC static_cast<short> \
  (WriteX_Level::DEBUG | \
  WriteX_Level::INFO)