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

    void loop();
public:
    WriteX(const std::string&);
    ~WriteX();
    void log(const std::string&, int);
};