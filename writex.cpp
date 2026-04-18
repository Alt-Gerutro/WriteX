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

WriteX::WriteX(const std::string& name, const std::string& filepath) : 
bgthread(&WriteX::loop, this), logger_name(name) {
  log_file.open(filepath, std::ios_base::out);
  fmt = "[%N] [%F %l] [%L] %M";
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

void WriteX::setFormat(std::string& fstr) {
  std::lock_guard<std::mutex> lock(mtx);
  fmt = fstr;
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
