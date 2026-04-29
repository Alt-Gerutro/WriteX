/**
 * @file writex.cpp
 * @author Alt-Gerutro (Gerutro) (gerutrogame@gmail.com)
 * @brief Code file
 * @version 0.3.0
 * @date 2026-04-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <array>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <writex.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <thread>

WriteX::Builder::Builder(const std::string_view name) : nm(name) {}

WriteX::Builder& WriteX::Builder::format(const std::string_view format_string) {
  fmt = format_string;
  return *this;
}

WriteX::Builder& WriteX::Builder::filter(const short filter) {
  ftr = filter;
  return *this;
}

WriteX::Builder& WriteX::Builder::newline(const bool newline) {
  nl = newline;
  return *this;
}

WriteX::Builder& WriteX::Builder::output_stream(const std::shared_ptr<std::ostream>& stream) {
  if (stream != nullptr) {
    output = stream;
  } else {
    output = std::shared_ptr<std::ostream> (&std::cout, [](std::ostream*) {});
  }
  return *this;
}

std::shared_ptr<WriteX> WriteX::Builder::build() {
  return std::make_shared<WriteX>(*this);
}

void WriteX::enq_msg(std::string msg) {
  {
    std::lock_guard<std::mutex> lock(mtx);
    msg_queue.push(std::move(msg));
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

      *ostream << s;

      lock.lock();
    }

    cv.notify_all();

    if (stop_flag && msg_queue.empty()) break;
  }
}

WriteX::WriteX(WriteX::Builder& builder) :
  logger_name(builder.nm),
  fmt(builder.fmt),
  filter_level(builder.ftr),
  add_newline(builder.nl)
{
  if (builder.output != nullptr) {
    ostream = builder.output;
  } else {
    ostream = std::shared_ptr<std::ostream>(&std::cout, [](auto*){});
  }
  bgthread = std::thread(&WriteX::loop, this);
}

WriteX::~WriteX() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop_flag = true;
  }
  cv.notify_all();

  if (bgthread.joinable()) bgthread.join();
}

std::string_view WriteX::levelToString(const WriteX_Level& level) {
  static constexpr std::array<std::string_view, 6> str_arr {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "UNKNOWN"};

  switch (level) {
    case WriteX_Level::DEBUG:   return str_arr[0];
    case WriteX_Level::INFO:    return str_arr[1];
    case WriteX_Level::WARNING: return str_arr[2];
    case WriteX_Level::ERROR:   return str_arr[3];
    case WriteX_Level::FATAL:   return str_arr[4];
    default:                    return str_arr[5];
  }
}

void WriteX::switchNewLine() {
  std::lock_guard<std::mutex> lock_flag(mtx);
  add_newline = !add_newline;
}

void WriteX::setFilter(std::underlying_type_t<WriteX_Level> filter) {
  std::lock_guard<std::mutex> lock(mtx);
  filter_level = filter;
}

short WriteX::getFilter() {
  std::lock_guard<std::mutex> lock(mtx);
  return filter_level;
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
  bool cur_add_newline;
  {
    std::lock_guard<std::mutex> lock(mtx);
    cur_fmt = fmt;
    cur_add_newline = add_newline;
  }

  std::string res;
  res.reserve(256);

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
  if (cur_add_newline) res += '\n';

  return res;
}

void WriteX::flush() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [this] {return msg_queue.empty() || stop_flag;});
}