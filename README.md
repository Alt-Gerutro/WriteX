<p align="center">
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue"></a>
  <img src="https://img.shields.io/badge/Project Version-0.3.0-white">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Standard-c++20-orange">
  <img src="https://img.shields.io/badge/Build-CMake_3.14+-orange">
  <img src="https://img.shields.io/badge/Depencies-none-green">
</p>


# WriteX

WriteX - Простой логгер на C++. Поддерживает многопоточность и асинхронность.  

---

## Оглавление

[установка](#установка)  
[использование](#использование)  
[фильтры](#фильтры)  
[документация](#документация)

---

## Установка

Чтобы установить библиотеку достаточно добавить файл `writex.cpp` и `writex.hpp` в свой проект. Ну или скомпилировать библиотеку заранее через `CMakeLists.txt` в проекте WriteX и добавить компиляцию с файлом библиотеки.

## Использование

Чтобы использовать библиотеку нужно подключить `writex.hpp` и скомпилировать программу вместе с скомпилированной библиотекой.

```cpp
#include "writex.hpp"

int main(int argc, char** argv) {
  auto logger = 
  WriteX::Builder("Name")
    .format("[%N] [%F %f:%l] [%L] %M")
    .filter(WRITEX_ALL_LEVELS)
    .newline(true)
    .output_stream(std::shared_ptr<std::ostream>(&std::cout, [](auto*){}))
    .build();
  LOG_INFO(logger, "Hello");
  LOG_FATAL(logger, "Fatal error");
  return 0;
}
```

Нужно помнить, что shared указатель вызывает стандартную функцию `delete` для объектов, на которые больше не ссылается ни один указатель. Это может привести к падению программы, если передать в него `std::cout`.  
Для таких случаев можно использовать создание объекта shared указателя с пустым деструктором  
`std::shared_ptr<std::ostream>(&std::cout, [](auto*){})`

```cpp
// ...

auto logger = WriteX::Builder("Name")
  .output_stream(std::shared_ptr<std::ostream>(&std::cout, [](auto*){}))
  .build()

// ...
```

## Фильтры

Фильтр(в данном контексте) - число, представляющее собой информацию о включённых и выключенных типах логирования. 

Объявление перечисления

```cpp
enum class WriteX_Level {
  DEBUG = (1 << 0), 
  INFO = (1 << 1), 
  WARNING = (1 << 2), 
  ERROR = (1 << 3), 
  FATAL = (1 << 4)
};
```

то есть каждый бит числа фильтра сходится с данным перечислением.  
Например, если `filter = 31`, то это значит, что все уровни логирования включены, так как если перевести это число в двоичное, то получится `00011111`.

Фильтрация проводится обычной проверкой на соответствие бита в этом числе и бита типа логирования.  

Для управления этим числом добавлены функции
```cpp
void WriteX::setFilter(short filter);

short WriteX::getFilter();
```

первая ставит, вторая выдаёт.  

Добавлена перегрузка оператора побитового или, для удобства, но возвращает она объект типа `WriteX_Level`, так что нужно его прикастовать.  

Есть макросы для быстрого получения чисел.
- `ALL_LEVELS` - все уровни
- `TO_<LEVEL>` - все уровни от FATAL до этого уровня.
- `TO_<LEVEL>_ASC` - все уровни от DEBUG до этого уровня

> [!IMPORTANT]
> Стоит учесть, что для уровня Fatal не определён макрос `TO_<LEVEL>`, а для debug - `TO_<LEVEL>_ASC`.
> Так же все макросы `TO_*` не работают с пользовательскими типами и числами, не являющимися степенью двойки. Например числа `321`, `35` и прочие.  

## Документация
Полная документация доступна на [GitHub Pages](https://alt-gerutro.github.io/WriteX/).  
Так же можно собрать документацию вручную через скрипт `scripts/docs_generate.sh`: по пути `docs/html` сгенерируется файл `index.html`, который нужно открыть в браузере. Или можно сразу запустить `scripts/docs_open.sh`, который откроет этот файл в браузере по умолчанию, если сможет определить команду для запуска.  

> [!IMPORTANT]
> Для генерации вручную потребуется установить `doxygen`.  
