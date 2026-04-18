#include "writex.hpp"

WriteX l("Melon", "file.log");


int main(int argc, char** argv) {
  
  l.setFilter(TO_WARNING_ASC);

  LOG_DEBUG(l, "Debug message");
  LOG_INFO(l, "Info message");
  LOG_WARNING(l, "Warning message");
  LOG_ERROR(l, "Error message");
  LOG_FATAL(l, "Fatal message");

  return 0;
}