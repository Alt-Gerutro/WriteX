/**
 * @file example_simple.cpp
 * @author Alt-Gerutro (Gerutro) (gerutrogame@gmail.com)
 * @brief Simple logger example
 * @version 0.3.0
 * @date 2026-04-27
 */

#include <writex.hpp>

int main() {
  auto logger = WriteX::Builder("Main").build();
  WRITEX_LOG_INFO(logger, "Running program...");
  WRITEX_LOG_WARNING(logger, "Some warning: {}", "Text of some warning");
  return 0;
}