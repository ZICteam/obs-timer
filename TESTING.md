# Руководство по тестированию

## Текущее состояние

В версии 1.0.0 автоматические тесты ещё не реализованы. Данный файл описывает план тестирования для будущих версий.

## Ручное тестирование

### Чек-лист перед релизом

#### Основная функциональность
- [ ] Плагин загружается в OBS без ошибок
- [ ] Источник "Countdown Timer" появляется в списке
- [ ] Таймер создаётся без ошибок
- [ ] Таймер отображается на сцене
- [ ] Обратный отсчёт работает корректно
- [ ] Формат времени MM:SS отображается правильно

#### Настройки
- [ ] Изменение длительности применяется
- [ ] Изменение шрифта работает
- [ ] Изменение размера шрифта работает
- [ ] Изменение цвета применяется
- [ ] Настройки fade in/out работают
- [ ] Все действия по завершении работают:
  - [ ] None - таймер останавливается на 00:00
  - [ ] Hide - источник скрывается
  - [ ] Reset - таймер перезапускается
  - [ ] Show Message - показывается сообщение

#### Эффекты
- [ ] Fade in плавно увеличивает прозрачность
- [ ] Fade out плавно уменьшает прозрачность
- [ ] Эффекты не вызывают мерцания
- [ ] Таймер без эффектов отображается сразу

#### Граничные случаи
- [ ] Минимальная длительность (1 сек) работает
- [ ] Максимальная длительность (7200 сек) работает
- [ ] Минимальный размер шрифта (12) читаем
- [ ] Максимальный размер шрифта (200) отображается
- [ ] Пустое название шрифта обрабатывается
- [ ] Несуществующий шрифт обрабатывается

#### Производительность
- [ ] FPS не падает при добавлении таймера
- [ ] Множественные таймеры не вызывают лагов
- [ ] Память корректно освобождается при удалении

#### Локализация
- [ ] Английский язык отображается правильно
- [ ] Русский язык отображается правильно
- [ ] Переключение языка в OBS обновляет названия

#### Совместимость
- [ ] Windows 10/11 - работает
- [ ] Linux (Ubuntu/Fedora) - работает
- [ ] macOS - работает
- [ ] OBS 28.x - работает
- [ ] OBS 29.x - работает
- [ ] OBS 30.x - работает

### Тестовые сценарии

#### Сценарий 1: Создание простого таймера
1. Открыть OBS Studio
2. Добавить источник "Countdown Timer"
3. Установить Duration: 60
4. Нажать OK
5. **Ожидается:** Таймер отображается с 01:00

#### Сценарий 2: Изменение внешнего вида
1. Создать таймер (Duration: 30)
2. ПКМ → Свойства
3. Изменить Font Size: 96
4. Изменить Color: Красный
5. Нажать OK
6. **Ожидается:** Таймер стал большим и красным

#### Сценарий 3: Эффекты fade
1. Создать таймер (Duration: 10)
2. Установить Fade In: 2.0
3. Установить Fade Out: 2.0
4. Запустить таймер
5. **Ожидается:** 
   - Первые 2 секунды - плавное появление
   - Последние 2 секунды - плавное исчезновение

#### Сценарий 4: Автоскрытие
1. Создать таймер (Duration: 5)
2. Установить Action: Hide Source
3. Запустить таймер
4. Дождаться завершения
5. **Ожидается:** Источник автоматически скрывается

#### Сценарий 5: Циклический таймер
1. Создать таймер (Duration: 5)
2. Установить Action: Reset and Restart
3. Запустить таймер
4. Дождаться завершения
5. **Ожидается:** Таймер автоматически начинает новый отсчёт

---

## Будущие автоматические тесты

### Unit тесты

```cpp
// tests/timer_tests.cpp

#include "timer-plugin.h"
#include <gtest/gtest.h>

// Тесты форматирования времени
TEST(TimerTests, FormatTime_ZeroSeconds) {
    EXPECT_EQ(timer_format_time(0), "00:00");
}

TEST(TimerTests, FormatTime_OneMinute) {
    EXPECT_EQ(timer_format_time(60), "01:00");
}

TEST(TimerTests, FormatTime_TenMinutes) {
    EXPECT_EQ(timer_format_time(600), "10:00");
}

TEST(TimerTests, FormatTime_OneHour) {
    EXPECT_EQ(timer_format_time(3600), "60:00");
}

// Тесты состояния таймера
TEST(TimerTests, InitialState_IsStopped) {
    timer_source context = {};
    EXPECT_EQ(context.state, TIMER_STATE_STOPPED);
}

TEST(TimerTests, StartTimer_ChangesState) {
    timer_source context = {};
    context.duration_seconds = 60;
    timer_start(&context);
    EXPECT_EQ(context.state, TIMER_STATE_RUNNING);
}

TEST(TimerTests, PauseTimer_ChangesState) {
    timer_source context = {};
    context.state = TIMER_STATE_RUNNING;
    timer_pause(&context);
    EXPECT_EQ(context.state, TIMER_STATE_PAUSED);
}

// Тесты действий
TEST(TimerTests, ActionFromString_None) {
    EXPECT_EQ(timer_action_from_string("none"), TIMER_ACTION_NONE);
}

TEST(TimerTests, ActionFromString_Hide) {
    EXPECT_EQ(timer_action_from_string("hide"), TIMER_ACTION_HIDE);
}

// Тесты прозрачности
TEST(TimerTests, CalculateAlpha_NoFade) {
    timer_source context = {};
    context.fade_in_duration = 0;
    context.fade_out_duration = 0;
    context.duration_seconds = 60;
    
    EXPECT_FLOAT_EQ(timer_calculate_alpha(&context, 30), 1.0f);
}

TEST(TimerTests, CalculateAlpha_FadeIn) {
    timer_source context = {};
    context.fade_in_duration = 2.0f;
    context.fade_out_duration = 0;
    context.duration_seconds = 60;
    
    EXPECT_FLOAT_EQ(timer_calculate_alpha(&context, 1.0f), 0.5f);
}
```

### Интеграционные тесты

```cpp
// tests/integration_tests.cpp

TEST(IntegrationTests, CreateAndDestroySource) {
    // Создание источника
    obs_data_t *settings = obs_data_create();
    obs_source_t *source = obs_source_create("timer_source", "test", settings, nullptr);
    
    ASSERT_NE(source, nullptr);
    
    // Удаление
    obs_source_release(source);
    obs_data_release(settings);
}

TEST(IntegrationTests, UpdateProperties) {
    obs_data_t *settings = obs_data_create();
    obs_data_set_int(settings, "duration", 120);
    obs_data_set_string(settings, "font", "Arial");
    
    obs_source_t *source = obs_source_create("timer_source", "test", settings, nullptr);
    // Проверить, что настройки применились
    
    obs_source_release(source);
    obs_data_release(settings);
}
```

### Настройка тестов

```cmake
# CMakeLists.txt для тестов

enable_testing()

find_package(GTest REQUIRED)

add_executable(timer_tests
    tests/timer_tests.cpp
    tests/integration_tests.cpp
    src/timer-plugin.cpp
)

target_link_libraries(timer_tests
    GTest::GTest
    GTest::Main
    OBS::libobs
)

add_test(NAME TimerTests COMMAND timer_tests)
```

---

## Continuous Integration

### GitHub Actions пример

```yaml
# .github/workflows/test.yml

name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [windows-latest, ubuntu-latest, macos-latest]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install dependencies
      run: |
        # Установка OBS и зависимостей
    
    - name: Build
      run: |
        mkdir build && cd build
        cmake ..
        cmake --build .
    
    - name: Run tests
      run: |
        cd build
        ctest --output-on-failure
```

---

## Инструменты

### Рекомендуемые инструменты для тестирования:

- **Google Test** - unit тесты
- **Valgrind** - проверка утечек памяти (Linux)
- **Visual Studio Memory Profiler** - профилирование (Windows)
- **AddressSanitizer** - поиск багов с памятью
- **OBS Studio в debug режиме** - для интеграционных тестов

---

## Внесение вклада

При добавлении новой функциональности:
1. Добавьте unit тесты
2. Обновите чек-лист ручного тестирования
3. Проверьте все существующие тесты проходят
4. Документируйте новые тестовые сценарии
