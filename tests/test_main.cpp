#include <catch2/catch_test_macros.hpp>
#include <writex.hpp>

WriteX l("Format", "file.log");

TEST_CASE("Format case", "[format]") {
  std::string oldFmt = l.getFormat();
  l.setFormat("%N %L %M %F %f %l %% %o");
  CHECK(l.format_msg("Hello {}", "World!") == "Hello World!");
  CHECK(l.format_msg("Hellosths {}sth", "World!") == "Hellosths World!sth");
  CHECK(l.format(WriteX_Level::DEBUG, "Hello", "FILE", "FUNC", 1) == "Format DEBUG Hello FILE FUNC 1 % %o");
  CHECK(l.format(WriteX_Level::INFO, "Hello1", "FILE1", "FUNC1", 2) == "Format INFO Hello1 FILE1 FUNC1 2 % %o");
  CHECK(l.format(WriteX_Level::WARNING, "Hello2", "FILE2", "FUNC2", 3) == "Format WARNING Hello2 FILE2 FUNC2 3 % %o");
  CHECK(l.format(WriteX_Level::ERROR, "Hello3", "FILE3", "FUNC3", 4) == "Format ERROR Hello3 FILE3 FUNC3 4 % %o");
  CHECK(l.format(WriteX_Level::FATAL, "Hello4", "FILE4", "FUNC4", 5) == "Format FATAL Hello4 FILE4 FUNC4 5 % %o");
  CHECK(l.format(WriteX_Level(1882), "Hello5", "FILE5", "FUNC5", 6) == "Format UNKNOWN Hello5 FILE5 FUNC5 6 % %o");

  l.setFormat("%N %F %");
  CHECK(l.format(WriteX_Level::INFO, "Hello", "FILE", "FUNC", 1) == "Format FILE %");
  CHECK(l.format(WriteX_Level::INFO, "Hello", "FILE%s", "FUNC", 1) == "Format FILE%s %");
}
