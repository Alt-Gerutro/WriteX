#include <writex.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <thread>
#include <string>
#include <type_traits>

const WriteX_Level operator|(WriteX_Level a, WriteX_Level b) {
  using T = std::underlying_type_t<WriteX_Level>;
  return static_cast<WriteX_Level>(static_cast<T>(a) | static_cast<T>(b));
}


WriteX::WriteX(const std::string& name, std::ostream& _ostream) :
bgthread(&WriteX::loop, this), logger_name(name), ostream(_ostream) {
  filter_level = WRITEX_ALL_LEVELS;
  fmt = "[%N] [%F %f:%l] [%L] %M";
}

WriteX::WriteX(const std::string& name, std::ostream& _ostream, const std::string& format) :
bgthread(&WriteX::loop, this), logger_name(name), ostream(_ostream), fmt(format) {
  filter_level = WRITEX_ALL_LEVELS;
}

WriteX::WriteX(const std::string& name, std::ostream& _ostream, short filter) :
bgthread(&WriteX::loop, this), logger_name(name), ostream(_ostream), filter_level(filter) {
  fmt = "[%N] [%F %f:%l] [%L] %M";
}

WriteX::WriteX(const std::string& name, std::ostream& _ostream, const std::string&format, short filter) :
bgthread(&WriteX::loop, this), logger_name(name), ostream(_ostream), fmt(format), filter_level(filter) {}

WriteX::~WriteX() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop_flag = true;
  }
  cv.notify_all();

  if (bgthread.joinable()) bgthread.join();
}

void WriteX::setFormat(std::string format_string) {
  std::lock_guard<std::mutex> lock(mtx);
  fmt = format_string;
}

std::string WriteX::getFormat() {
  std::lock_guard<std::mutex> lock(mtx);
  return fmt;
}

std::string WriteX::format(WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line) {
  std::string cur_fmt;
  {
    std::lock_guard<std::mutex> lock(mtx);
    cur_fmt = fmt;
  }

  std::string res;
  for (size_t i = 0; i < cur_fmt.size(); ++i) {
    if (cur_fmt[i] == '%' && i+1 < cur_fmt.size()) {
      char c = cur_fmt[i+1];
      switch (c) {
        case 'N': res += logger_name; break;
        case 'L': res += levelToString(lvl); break;
        case 'M': res += msg; break;

        case 'F': res += file; break;
        case 'f': res += func; break;
        case 'l': res += std::to_string(line); break;
        case '%': res += '%'; break;
        default: {
          res += '%';
          res += c;
          break;
        }
      }
      ++i;
    } else {
      res += cur_fmt[i];
    }
  }
  return res;
}

void WriteX::setFilter(short filter) {
  std::lock_guard<std::mutex> lock(mtx);
  filter_level = filter;
}

short WriteX::getFilter() {
  std::lock_guard<std::mutex> lock(mtx);
  return filter_level;
}

std::string WriteX::levelToString(const WriteX_Level& l) const {
  switch (l) {
    case WriteX_Level::DEBUG: return "DEBUG";
    case WriteX_Level::INFO: return "INFO";
    case WriteX_Level::WARNING: return "WARNING";
    case WriteX_Level::ERROR: return "ERROR";
    case WriteX_Level::FATAL: return "FATAL";
    default: return "UNKNOWN";
  }
}

void WriteX::loop() {
  while(true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !msg_queue.empty() || stop_flag; });

    while (!msg_queue.empty()) {
      std::string s = std::move(msg_queue.front());
      msg_queue.pop();

      bool cur_add_newline_flag = add_newline;

      lock.unlock();

      if (cur_add_newline_flag) {
        ostream << s << std::endl;
      } else {
        ostream << s;
      }

      lock.lock();
    }

    cv.notify_all();

    if (stop_flag && msg_queue.empty()) break;
  }
}

void WriteX::enq_msg(std::string& msg) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    msg_queue.push(std::move(msg));
  }

  cv.notify_one();
}

void WriteX::flush() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [this] {return msg_queue.empty() | stop_flag;});
}

void WriteX::switchNewLine() {
  std::lock_guard<std::mutex> lock_flag(mtx);
  add_newline = !add_newline;
}