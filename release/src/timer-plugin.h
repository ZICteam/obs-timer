#pragma once

#include <obs-module.h>
#include <obs-source.h>
#include <graphics/vec4.h>
#include <util/platform.h>
#include <string>

/**
 * Timer Plugin для OBS Studio
 * 
 * Этот заголовочный файл содержит определения для плагина таймера обратного отсчёта.
 * В будущих версиях код будет разделён на несколько файлов для лучшей организации.
 */

// Версия плагина
#define TIMER_PLUGIN_VERSION "1.0.0"
#define TIMER_PLUGIN_VERSION_MAJOR 1
#define TIMER_PLUGIN_VERSION_MINOR 0
#define TIMER_PLUGIN_VERSION_PATCH 0

// Константы
#define TIMER_MIN_DURATION 1        // Минимальная длительность (секунды)
#define TIMER_MAX_DURATION 7200     // Максимальная длительность (секунды) - 2 часа
#define TIMER_MIN_FONT_SIZE 12      // Минимальный размер шрифта
#define TIMER_MAX_FONT_SIZE 200     // Максимальный размер шрифта
#define TIMER_MIN_FADE 0.0f         // Минимальная длительность fade (секунды)
#define TIMER_MAX_FADE 10.0f        // Максимальная длительность fade (секунды)

// Идентификаторы свойств
#define PROP_DURATION "duration"
#define PROP_FONT "font"
#define PROP_FONT_SIZE "font_size"
#define PROP_COLOR "color"
#define PROP_FADE_IN "fade_in"
#define PROP_FADE_OUT "fade_out"
#define PROP_FINISH_ACTION "finish_action"
#define PROP_FINISH_MESSAGE "finish_message"
#define PROP_START_BUTTON "start_button"
#define PROP_PAUSE_BUTTON "pause_button"
#define PROP_RESET_BUTTON "reset_button"

// Значения по умолчанию
#define DEFAULT_DURATION 60
#define DEFAULT_FONT "Arial"
#define DEFAULT_FONT_SIZE 72
#define DEFAULT_COLOR 0xFFFFFFFF
#define DEFAULT_FADE_IN 0.5
#define DEFAULT_FADE_OUT 0.5
#define DEFAULT_ACTION "none"
#define DEFAULT_MESSAGE "TIME'S UP!"

/**
 * Действия, выполняемые по завершении таймера
 */
enum timer_finish_action {
	TIMER_ACTION_NONE,           // Ничего не делать
	TIMER_ACTION_HIDE,           // Скрыть источник
	TIMER_ACTION_RESET,          // Сбросить и перезапустить
	TIMER_ACTION_SHOW_MESSAGE    // Показать сообщение
};

/**
 * Состояние таймера
 */
enum timer_state {
	TIMER_STATE_STOPPED,    // Остановлен
	TIMER_STATE_RUNNING,    // Запущен
	TIMER_STATE_PAUSED,     // На паузе
	TIMER_STATE_FINISHED    // Завершён
};

/**
 * Основная структура данных источника таймера
 */
struct timer_source {
	obs_source_t *source;           // Ссылка на источник OBS
	
	// Параметры таймера
	int duration_seconds;           // Установленная длительность
	int remaining_seconds;          // Оставшееся время
	uint64_t start_time;            // Время начала (наносекунды)
	uint64_t pause_time;            // Время паузы
	timer_state state;              // Текущее состояние
	
	// Параметры отображения
	char *font_name;                // Название шрифта
	int font_size;                  // Размер шрифта
	uint32_t color;                 // Цвет текста (RGBA)
	
	// Эффекты
	float fade_in_duration;         // Длительность появления
	float fade_out_duration;        // Длительность затухания
	float current_alpha;            // Текущая прозрачность (0.0-1.0)
	
	// Действие по завершении
	timer_finish_action finish_action;
	char *finish_message;           // Сообщение для показа
	
	// Рендеринг
	gs_texture_t *texture;          // Текстура с текстом
	uint32_t cx;                    // Ширина текстуры
	uint32_t cy;                    // Высота текстуры
	bool needs_update;              // Требуется обновление текстуры
};

// Функции для работы с таймером (будут реализованы при рефакторинге)

/**
 * Форматирование времени в строку
 * @param seconds Количество секунд
 * @return Строка в формате MM:SS
 */
std::string timer_format_time(int seconds);

/**
 * Запуск таймера
 * @param context Контекст источника
 */
void timer_start(struct timer_source *context);

/**
 * Остановка таймера
 * @param context Контекст источника
 */
void timer_stop(struct timer_source *context);

/**
 * Пауза таймера
 * @param context Контекст источника
 */
void timer_pause(struct timer_source *context);

/**
 * Сброс таймера
 * @param context Контекст источника
 */
void timer_reset(struct timer_source *context);

/**
 * Обновление состояния таймера
 * @param context Контекст источника
 * @param elapsed_seconds Прошедшее время
 */
void timer_update_state(struct timer_source *context, int elapsed_seconds);

/**
 * Преобразование строки действия в enum
 * @param action_str Строка действия
 * @return Enum действия
 */
timer_finish_action timer_action_from_string(const char *action_str);

/**
 * Вычисление текущей прозрачности с учётом fade эффектов
 * @param context Контекст источника
 * @param elapsed_seconds Прошедшее время
 * @return Значение прозрачности (0.0-1.0)
 */
float timer_calculate_alpha(struct timer_source *context, float elapsed_seconds);

/**
 * Создание текстуры с текстом таймера
 * @param context Контекст источника
 * @param text Текст для отображения
 * @return true если успешно
 */
bool timer_create_texture(struct timer_source *context, const char *text);

/**
 * Освобождение ресурсов текстуры
 * @param context Контекст источника
 */
void timer_free_texture(struct timer_source *context);

// Callback функции OBS источника

/**
 * Получение имени источника
 */
const char *timer_source_get_name(void *unused);

/**
 * Создание источника
 */
void *timer_source_create(obs_data_t *settings, obs_source_t *source);

/**
 * Удаление источника
 */
void timer_source_destroy(void *data);

/**
 * Обновление свойств источника
 */
void timer_source_update(void *data, obs_data_t *settings);

/**
 * Получение свойств для UI
 */
obs_properties_t *timer_source_properties(void *data);

/**
 * Установка значений по умолчанию
 */
void timer_source_defaults(obs_data_t *settings);

/**
 * Тик для обновления логики
 */
void timer_source_tick(void *data, float seconds);

/**
 * Видео тик для рендеринга
 */
void timer_source_video_tick(void *data, float seconds);

/**
 * Рендеринг источника
 */
void timer_source_render(void *data, gs_effect_t *effect);

/**
 * Получение ширины источника
 */
uint32_t timer_source_get_width(void *data);

/**
 * Получение высоты источника
 */
uint32_t timer_source_get_height(void *data);

// Callback для кнопок

/**
 * Callback для кнопки Start
 */
bool timer_button_start_clicked(obs_properties_t *props, obs_property_t *property, void *data);

/**
 * Callback для кнопки Pause
 */
bool timer_button_pause_clicked(obs_properties_t *props, obs_property_t *property, void *data);

/**
 * Callback для кнопки Reset
 */
bool timer_button_reset_clicked(obs_properties_t *props, obs_property_t *property, void *data);
