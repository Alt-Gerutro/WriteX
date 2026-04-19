#include <catch2/catch_test_macros.hpp>
#include <writex.hpp>

WriteX l("Format", "file.log");

TEST_CASE("Format case", "[format]") {
  std::string oldFmt = l.getFormat();
  l.setFormat("%N %L %M %F %f %l %% %o");
  CHECK(l.getFormat() == "%N %L %M %F %f %l %% %o");

  CHECK(l.format_msg("Hello {}", "World!") == "Hello World!");
  CHECK(l.format_msg("Hellosths {}sth", "World!") == "Hellosths World!sth");

  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "Format DEBUG MESSAGE FILE FUNC 1 % %o");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "Format INFO MESSAGE1 FILE1 FUNC1 2 % %o");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "Format WARNING MESSAGE2 FILE2 FUNC2 3 % %o");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "Format ERROR MESSAGE3 FILE3 FUNC3 4 % %o");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "Format FATAL MESSAGE4 FILE4 FUNC4 5 % %o");
  CHECK(l.format(WriteX_Level(1882), "MESSAGE5", "FILE5", "FUNC5", 6) == "Format UNKNOWN MESSAGE5 FILE5 FUNC5 6 % %o");

  l.setFormat("%N");
  CHECK(l.getFormat() == "%N");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "Format");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "Format");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "Format");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "Format");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "Format");
  CHECK(l.format(WriteX_Level(842), "MESSAGE5", "FILE5", "FUNC5",6) == "Format");

  l.setFormat("%L");
  CHECK(l.getFormat() == "%L");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "DEBUG");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "INFO");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "WARNING");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "ERROR");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "FATAL");
  CHECK(l.format(WriteX_Level(325), "MESSAGE5", "FILE5", "FUNC5",6) == "UNKNOWN");

  l.setFormat("%M");
  CHECK(l.getFormat() == "%M");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "MESSAGE");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "MESSAGE1");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "MESSAGE2");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "MESSAGE3");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "MESSAGE4");
  CHECK(l.format(WriteX_Level(102), "MESSAGE5", "FILE5", "FUNC5",6) == "MESSAGE5");

  l.setFormat("%F");
  CHECK(l.getFormat() == "%F");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "FILE");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "FILE1");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "FILE2");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "FILE3");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "FILE4");
  CHECK(l.format(WriteX_Level(100), "MESSAGE5", "FILE5", "FUNC5",6) == "FILE5");

  l.setFormat("%f");
  CHECK(l.getFormat() == "%f");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "FUNC");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "FUNC1");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "FUNC2");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "FUNC3");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "FUNC4");
  CHECK(l.format(WriteX_Level(182), "MESSAGE5", "FILE5", "FUNC5",6) == "FUNC5");

  l.setFormat("%l");
  CHECK(l.getFormat() == "%l");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "1");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "2");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "3");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "4");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "5");
  CHECK(l.format(WriteX_Level(940), "MESSAGE5", "FILE5", "FUNC5",6) == "6");

  l.setFormat("%%");
  CHECK(l.getFormat() == "%%");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "%");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "%");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "%");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "%");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "%");
  CHECK(l.format(WriteX_Level(72), "MESSAGE5", "FILE5", "FUNC5",6) == "%");

  l.setFormat("%");
  CHECK(l.getFormat() == "%");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "%");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "%");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "%");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "%");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "%");
  CHECK(l.format(WriteX_Level(90), "MESSAGE5", "FILE5", "FUNC5",6) == "%");

  l.setFormat("[23] {d} {} ()");
  CHECK(l.getFormat() == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "[23] {d} {} ()");
  CHECK(l.format(WriteX_Level(910), "MESSAGE5", "FILE5", "FUNC5",6) == "[23] {d} {} ()");

  l.setFormat("%0");
  CHECK(l.getFormat() == "%0");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "%0");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "%0");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "%0");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "%0");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "%0");
  CHECK(l.format(WriteX_Level(91), "MESSAGE5", "FILE5", "FUNC5",6) == "%0");

  l.setFormat("");
  CHECK(l.format(WriteX_Level::DEBUG, "MESSAGE", "FILE", "FUNC", 1) == "");
  CHECK(l.format(WriteX_Level::INFO, "MESSAGE1", "FILE1", "FUNC1", 2) == "");
  CHECK(l.format(WriteX_Level::WARNING, "MESSAGE2", "FILE2", "FUNC2", 3) == "");
  CHECK(l.format(WriteX_Level::ERROR, "MESSAGE3", "FILE3", "FUNC3", 4) == "");
  CHECK(l.format(WriteX_Level::FATAL, "MESSAGE4", "FILE4", "FUNC4", 5) == "");
  CHECK(l.format(WriteX_Level(131), "MESSAGE5", "FILE5", "FUNC5", 6) == "");
}