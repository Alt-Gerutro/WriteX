/**
 * @file example_full_build.cpp
 * @author Alt-Gerutro (Gerutro) (gerutrogame@gmail.com)
 * @brief Full logger example
 * @version 0.3.0
 * @date 2026-04-27
 * 
 * @copyright Copyright (c) 2026
 */

 #include <writex.hpp>
 #include <iostream>

int main() {
  auto logger = WriteX::Builder("Main")
    .format("[%N] [%F %f:%l] [%L] %M")
    .filter(static_cast<short>(WriteX_Level::INFO))
    .output_stream(std::shared_ptr<std::ostream>(&std::cout, [](auto*){}))
    .newline(true)
    .build();
  WRITEX_LOG_INFO(logger, "Running program...");
  WRITEX_LOG_WARNING(logger, "Some warning: {}", "Text of some warning");
  return 0;
}