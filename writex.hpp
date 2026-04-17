#pragma once

#include <condition_variable>
#include <fstream>
#include <queue>
#include <thread>

/**
 *  added in @version 0.0.1
 */
class WriteX {
private:
    std::thread bgthread;
    std::condition_variable cv;
    bool stop_flag {false};

    std::queue<std::string> msg_queue;
    std::mutex mtx;
    std::ofstream file;

    std::string general_fmt;

    void enq_msg(std::string&);
    void loop();
public:
    WriteX(const std::string&, const std::string&);
    ~WriteX();
    void log(const std::string&, int);

    template<typename... Args>
    void logf(Args&&... args) {
      std::string formatted = std::vformat(general_fmt, std::make_format_args(args...));
      enq_msg(formatted);
    }
};