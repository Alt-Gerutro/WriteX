/**
 * @file example_full_build.cpp
 * @author Alt-Gerutro (Gerutro) (gerutrogame@gmail.com)
 * @brief Multi logger example
 * @version 0.3.0
 * @date 2026-04-28
 * 
 * @copyright Copyright (c) 2026
 */

#include <string>
#include <writex.hpp>
#include <iostream>

int main() {
  auto logger1 = WriteX::Builder("Main")
    .output_stream(std::shared_ptr<std::ostream>(&std::cout, [](auto*){}))
    .build();

  auto logger2 = WriteX::Builder("Not so main")
    .output_stream(std::shared_ptr<std::ostream>(&std::cout, [](auto*){}))
    .build();
  
  WRITEX_LOG_INFO(logger1, "Hello from logger1!");
  WRITEX_LOG_INFO(logger2, "Hello from logger2!");
  return 0;
}