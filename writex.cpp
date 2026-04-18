#include "writex.hpp"
#include <condition_variable>
#include <fstream>
#include <ios>
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

WriteX::WriteX(const std::string& name, const std::string& filepath) : 
bgthread(&WriteX::loop, this), logger_name(name) {
  log_file.open(filepath, std::ios_base::out);
  filter_level = ALL_LEVELS;
  fmt = "[%N] [%F %f:%l] [%L] %M";
}

WriteX::WriteX(const std::string& name, const std::string& format, short filter, const std::string& filepath) : 
bgthread(&WriteX::loop, this), logger_name(name), fmt(format), filter_level(filter) {
  log_file.open(filepath, std::ios_base::out);
}

WriteX::WriteX(const std::string& name, short filter, const std::string& filepath) : 
bgthread(&WriteX::loop, this), logger_name(name), filter_level(filter) {
  log_file.open(filepath, std::ios_base::out);
}

WriteX::~WriteX() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop_flag = true;
  }
  cv.notify_all();

  if (bgthread.joinable()) bgthread.join();
  if (log_file.is_open()) log_file.close();
}

void WriteX::setFormat(std::string& format_string) {
  std::lock_guard<std::mutex> lock(mtx);
  fmt = format_string;
}

std::string WriteX::getFormat() {
  std::lock_guard<std::mutex> lock(mtx);
  return fmt;
}

std::string WriteX::format(WriteX_Level lvl, const std::string& msg, const char* file, const char* func, int line) {
  std::string res;
  for (size_t i = 0; i < fmt.size(); ++i) {
    if (fmt[i] == '%' && i+1 < fmt.size()) {
      switch (fmt[++i]) {
        case 'N': res += logger_name; break;
        case 'L': res += levelToString(lvl); break;
        case 'M': res += msg; break;

        case 'F': res += file; break;
        case 'f': res += func; break;
        case 'l': res += std::to_string(line); break;
        default: res += '%'; res += msg[++i]; 
      }
    } else {
      res += fmt[i];
    }
  }
  return res;
}

void WriteX::setFilter(short filter) {
  std::lock_guard<std::mutex> lock(mtx);
  filter_level = filter;
}

short WriteX::getFilretInt() {
  std::lock_guard<std::mutex> lock(mtx);
  return filter_level;
}

std::string WriteX::levelToString(WriteX_Level& l) {
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

      lock.unlock();

      if (log_file.is_open()) {
        log_file << s << std::endl;
      }
      std::cout << s << std::endl;

      lock.lock();
    }

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
