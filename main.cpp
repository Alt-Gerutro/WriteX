#include "writex.hpp"
#include <thread>

WriteX l("Melon", "file.log");

void func1() {
  LOG_DEBUG(l, "Hello from func1");
}

int main(int argc, char** argv) {
  std::thread tr1 (&func1);
  LOG_DEBUG(l, "Hello from main");

  tr1.join();

  return 0;
}