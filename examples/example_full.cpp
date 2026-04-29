/**
 * @file example_full_build.cpp
 * @author Alt-Gerutro (Gerutro) (gerutrogame@gmail.com)
 * @brief Full logger example
 * @version 0.3.0
 * @date 2026-04-27
 * 
 * @copyright Copyright (c) 2026
 */

#include <string>
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
  WRITEX_LOG_ERROR(logger, "Some error: {}", "Error? Where?");
  WRITEX_LOG_FATAL(logger, "Some fatal error: {}", "Hello there");

  logger->setFilter(static_cast<short>(WriteX_Level::ERROR | WriteX_Level::FATAL));

  WRITEX_LOG_INFO(logger, "Just repeating for example. This message was not printed.");
  WRITEX_LOG_WARNING(logger, "Some warning: {}", "Text of some warning");
  WRITEX_LOG_ERROR(logger, "Some error: {}", "Error? Where?");
  WRITEX_LOG_FATAL(logger, "Some fatal error: {}", "Hello there");

  logger->setFormat("I want other format. [%N] [%f:%l] [%L] > %M");
  logger->setFilter(WRITEX_ALL_LEVELS);

  WRITEX_LOG_INFO(logger, "Just repeating for example");
  WRITEX_LOG_WARNING(logger, "Some warning: {}", "Text of some warning");
  WRITEX_LOG_ERROR(logger, "Some error: {}", "Error? Where?");
  WRITEX_LOG_FATAL(logger, "Some fatal error: {}", "Hello there");

  WRITEX_LOG_INFO(logger, "Message with some numbers: {}", 2);
  WRITEX_LOG_INFO(logger, "Message with some numbers: {}", 21414);
  WRITEX_LOG_INFO(logger, "Message with some numbers and strings: {} = {}", "status", 2);
  return 0;
}