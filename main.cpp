#include "writex.hpp"

int main(int argc, char** argv) {
    WriteX logger("file.log");

    std::thread tr1(&WriteX::log, &logger, std::string("Text1"), 1);
    std::thread tr2(&WriteX::log, &logger, std::string("Text2"), 2);
    std::thread tr3(&WriteX::log, &logger, std::string("Text3"), 3);
    std::thread tr4(&WriteX::log, &logger, std::string("Text4"), 4);

    tr1.join();
    tr2.join();
    tr3.join();
    tr4.join();

    return 0;
}