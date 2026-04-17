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
#include <sstream>

WriteX::WriteX(const std::string& filepath) : bgthread(&WriteX::loop, this) {
  file.open(filepath, std::ios_base::out);
}

WriteX::~WriteX() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop_flag = true;
  }
  cv.notify_all();

  if (bgthread.joinable()) bgthread.join();
  if (file.is_open()) file.close();
}

void WriteX::log(const std::string& msg, int tid) {
  std::ostringstream oss;
  oss << "[Thread " << tid << "] " << msg;
  std::string formatted = oss.str();
  oss.str("");

  {
    std::lock_guard<std::mutex> lock(mtx);
    msg_queue.push(std::move(formatted));
  }

  cv.notify_one();
}

void WriteX::loop() {
  while(true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !msg_queue.empty() || stop_flag; });

    while (!msg_queue.empty()) {
      std::string s = std::move(msg_queue.front());
      msg_queue.pop();

      lock.unlock();

      if (file.is_open()) {
        file << s << std::endl;
      }
      std::cout << s << std::endl;

      lock.lock();
    }

    if (stop_flag && msg_queue.empty()) break;
  }
}