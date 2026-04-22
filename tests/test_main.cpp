#include <catch2/catch_test_macros.hpp>
#include <format>
#include <unordered_map>
#include <writex.hpp>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <thread>

static std::ostringstream fake_out;

WriteX l("Global", fake_out);

TEST_CASE("Format case", "[format]") {
  std::string oldFmt = l.getFormat();

  struct FormatTest {
    std::string format;
    std::vector<std::pair<WriteX_Level, std::string>> ex;
  };

  auto testFormat = [&](const FormatTest& ft) {
    l.setFormat(ft.format);
    CHECK(l.getFormat() == ft.format);
    for (const auto& [lvl, ex] : ft.ex) {
      INFO("Level: " << l.levelToString(lvl));
      CHECK(l.format(lvl, "MESSAGE", "FILE", "FUNC", 1) == ex);
    }
  };

  SECTION("Format Message specifiers") {
    CHECK(l.format_msg("Hello {} {}", "World") == "CANNOT FORMAT STRING: Hello {} {}");
    CHECK(l.format_msg("Hello {}", "World!") == "Hello World!");
  }

  SECTION("All format specifiers") {
    testFormat({
      "%N %L %M %F %f %l %% %0 %o %",
      {
        {WriteX_Level::DEBUG, "Global DEBUG MESSAGE FILE FUNC 1 % %0 %o %"},
        {WriteX_Level::INFO, "Global INFO MESSAGE FILE FUNC 1 % %0 %o %"},
        {WriteX_Level::WARNING, "Global WARNING MESSAGE FILE FUNC 1 % %0 %o %"},
        {WriteX_Level::ERROR, "Global ERROR MESSAGE FILE FUNC 1 % %0 %o %"},
        {WriteX_Level::FATAL, "Global FATAL MESSAGE FILE FUNC 1 % %0 %o %"},
        {WriteX_Level(123), "Global UNKNOWN MESSAGE FILE FUNC 1 % %0 %o %"},
      }
    });

    testFormat({
      "%N",
      {
        {WriteX_Level::DEBUG, "Global"},
        {WriteX_Level::INFO, "Global"},
        {WriteX_Level::WARNING, "Global"},
        {WriteX_Level::ERROR, "Global"},
        {WriteX_Level::FATAL, "Global"},
        {WriteX_Level(123), "Global"},
      }
    });

    testFormat({
      "%N",
      {
        {WriteX_Level::DEBUG, "Global"},
        {WriteX_Level::INFO, "Global"},
        {WriteX_Level::WARNING, "Global"},
        {WriteX_Level::ERROR, "Global"},
        {WriteX_Level::FATAL, "Global"},
        {WriteX_Level(123), "Global"},
      }
    });

    testFormat({
      "%L",
      {
        {WriteX_Level::DEBUG, "DEBUG"},
        {WriteX_Level::INFO, "INFO"},
        {WriteX_Level::WARNING, "WARNING"},
        {WriteX_Level::ERROR, "ERROR"},
        {WriteX_Level::FATAL, "FATAL"},
        {WriteX_Level(123), "UNKNOWN"},
      }
    });

    testFormat({
      "%M",
      {
        {WriteX_Level::DEBUG, "MESSAGE"},
        {WriteX_Level::INFO, "MESSAGE"},
        {WriteX_Level::WARNING, "MESSAGE"},
        {WriteX_Level::ERROR, "MESSAGE"},
        {WriteX_Level::FATAL, "MESSAGE"},
        {WriteX_Level(123), "MESSAGE"},
      }
    });

    testFormat({
      "%F",
      {
        {WriteX_Level::DEBUG, "FILE"},
        {WriteX_Level::INFO, "FILE"},
        {WriteX_Level::WARNING, "FILE"},
        {WriteX_Level::ERROR, "FILE"},
        {WriteX_Level::FATAL, "FILE"},
        {WriteX_Level(123), "FILE"},
      }
    });

    testFormat({
      "%f",
      {
        {WriteX_Level::DEBUG, "FUNC"},
        {WriteX_Level::INFO, "FUNC"},
        {WriteX_Level::WARNING, "FUNC"},
        {WriteX_Level::ERROR, "FUNC"},
        {WriteX_Level::FATAL, "FUNC"},
        {WriteX_Level(123), "FUNC"},
      }
    });

    testFormat({
      "%l",
      {
        {WriteX_Level::DEBUG, "1"},
        {WriteX_Level::INFO, "1"},
        {WriteX_Level::WARNING, "1"},
        {WriteX_Level::ERROR, "1"},
        {WriteX_Level::FATAL, "1"},
        {WriteX_Level(123), "1"},
      }
    });

    testFormat({
      "%%",
      {
        {WriteX_Level::DEBUG, "%"},
        {WriteX_Level::INFO, "%"},
        {WriteX_Level::WARNING, "%"},
        {WriteX_Level::ERROR, "%"},
        {WriteX_Level::FATAL, "%"},
        {WriteX_Level(123), "%"},
      }
    });

    testFormat({
      "%o",
      {
        {WriteX_Level::DEBUG, "%o"},
        {WriteX_Level::INFO, "%o"},
        {WriteX_Level::WARNING, "%o"},
        {WriteX_Level::ERROR, "%o"},
        {WriteX_Level::FATAL, "%o"},
        {WriteX_Level(123), "%o"},
      }
    });

    testFormat({
      "%0",
      {
        {WriteX_Level::DEBUG, "%0"},
        {WriteX_Level::INFO, "%0"},
        {WriteX_Level::WARNING, "%0"},
        {WriteX_Level::ERROR, "%0"},
        {WriteX_Level::FATAL, "%0"},
        {WriteX_Level(123), "%0"},
      }
    });

    testFormat({
      "%",
      {
        {WriteX_Level::DEBUG, "%"},
        {WriteX_Level::INFO, "%"},
        {WriteX_Level::WARNING, "%"},
        {WriteX_Level::ERROR, "%"},
        {WriteX_Level::FATAL, "%"},
        {WriteX_Level(123), "%"},
      }
    });

    testFormat({
      "",
      {
        {WriteX_Level::DEBUG, ""},
        {WriteX_Level::INFO, ""},
        {WriteX_Level::WARNING, ""},
        {WriteX_Level::ERROR, ""},
        {WriteX_Level::FATAL, ""},
        {WriteX_Level(123), ""},
      }
    });
  }

  l.setFormat(oldFmt);
}

TEST_CASE("Filter case", "[filter]") {
  std::string oldFmt = l.getFormat();
  short oldFil = l.getFilter();
  l.switchNewLine();

  l.setFormat("%L");
  
  struct FilterTest {
    short filter;
    std::vector<std::pair<WriteX_Level, bool>> outcomes;
  };

  std::vector<FilterTest> tests = {
    {WRITEX_TO_INFO, {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, true},
    {WriteX_Level::WARNING, true},
    {WriteX_Level::ERROR, true},
    {WriteX_Level::FATAL, true}
    }},
    {WRITEX_TO_WARNING, {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, true},
    {WriteX_Level::ERROR, true},
    {WriteX_Level::FATAL, true}
    }},
    {WRITEX_TO_ERROR, {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, true},
    {WriteX_Level::FATAL, true}
    }},
    {WRITEX_TO_ERROR_ASC, {
    {WriteX_Level::DEBUG, true},
    {WriteX_Level::INFO, true},
    {WriteX_Level::WARNING, true},
    {WriteX_Level::ERROR, true},
    {WriteX_Level::FATAL, false}
    }},
    {WRITEX_TO_WARNING_ASC, {
    {WriteX_Level::DEBUG, true},
    {WriteX_Level::INFO, true},
    {WriteX_Level::WARNING, true},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false}
    }},
    {WRITEX_TO_INFO_ASC, {
    {WriteX_Level::DEBUG, true},
    {WriteX_Level::INFO, true},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false}
    }},
    {static_cast<short>(WriteX_Level::DEBUG), {
    {WriteX_Level::DEBUG, true},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), false}
    }},
    {static_cast<short>(WriteX_Level::INFO), {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, true},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), false}
    }},
    {static_cast<short>(WriteX_Level::WARNING), {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, true},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), false}
    }},
    {static_cast<short>(WriteX_Level::ERROR), {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, true},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), false}
    }},
    {static_cast<short>(WriteX_Level::FATAL), {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, true},
    {WriteX_Level(1 << 10), false}
    }},
    {1 << 10, {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), true},
    {WriteX_Level(1 << 11), false},
    }},
    {1 << 11, {
    {WriteX_Level::DEBUG, false},
    {WriteX_Level::INFO, false},
    {WriteX_Level::WARNING, false},
    {WriteX_Level::ERROR, false},
    {WriteX_Level::FATAL, false},
    {WriteX_Level(1 << 10), false},
    {WriteX_Level(1 << 11), true},
    }}
  };

  for (const auto& ft : tests) {
    DYNAMIC_SECTION("Filter = " << ft.filter) {
      l.setFilter(ft.filter);
      for (const auto& [lvl, shouldLog] : ft.outcomes) {
        fake_out.str("");
        l.log(lvl, "MESSAGE", "FILE", "FUNC", 1);
        l.flush();
        std::string ex = shouldLog ? (l.levelToString(lvl)) : "";
        CHECK(fake_out.str() == ex);
      }
    }
  }
  
  l.setFormat(oldFmt);
  l.setFilter(oldFil);
  l.switchNewLine();
}

const std::string test_threads_msg = "Thread:{}, Message:{}";

void test_log(int thread_num, int messages_count) {
  for(int i = 0; i < messages_count; i++) {
    WRITEX_LOG_INFO(l, test_threads_msg, thread_num, i);
  }
}

TEST_CASE("Threads case", "[thread]") {
  std::string oldFmt = l.getFormat();
  l.setFormat("%M");

  fake_out.str("");
  REQUIRE(fake_out.str() == "");

  std::vector<std::thread> threads;

  int threads_count = 10;
  int messages_count = 100;

  for (int i = 0; i < threads_count; i++) {
    threads.push_back(std::thread {&test_log, i, messages_count});
  }

  for (int i = 0; i < threads_count; i++) {
    threads.back().join();
    threads.pop_back();
  }
  l.flush();

  std::vector<std::string> messages;

  {
    std::string item;
    std::istringstream iss {fake_out.str()};
    fake_out.str("");
    while (std::getline(iss, item, '\n')) {
      if (!item.empty()) {
        messages.push_back(item);
      }
    }
  }

  // Actual counts map
  std::unordered_map<std::string, int> actual_messages_map;

  for (int i = 0; i < messages.size(); i++) {
    actual_messages_map[messages[i]]++;
  }

  // Expected counts map
  std::unordered_map<std::string, int> expected_messages_map;

  for (int i = 0; i < threads_count; i++) {
    for (int j = 0; j < messages_count; j++) {
      // std::make_format_args leave dangling references, not critical.
      std::string key = std::vformat(test_threads_msg, std::make_format_args(i, j));
      expected_messages_map[key] = 1;
    }
  }

  // Checking size of expected and actual maps
  CHECK(expected_messages_map.size() == actual_messages_map.size());
  for (const auto& [key, expected_count] : expected_messages_map) {
    INFO("Key = " << key);
    CHECK(actual_messages_map[key] == expected_count);
  }

  l.setFormat(oldFmt);
}