#include "writex.hpp"

int main(int argc, char** argv) {
    WriteX l("file.log", "[Thread {}] {}");

    l.logf(1, "Hello");

    return 0;
}