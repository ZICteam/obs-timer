#include <obs-module.h>
#include <obs-source.h>
#include <graphics/vec4.h>
#include <util/platform.h>
#include <string>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-timer-plugin", "en-US")

MODULE_EXPORT const char *obs_module_description(void)
{
	return "Timer countdown plugin for OBS Studio";
}

// Структура данных таймера
struct timer_source {
	obs_source_t *source;
	
	// Параметры таймера
	int duration_seconds;        // Длительность таймера в секундах
	int remaining_seconds;       // Оставшееся время
	bool is_running;             // Запущен ли таймер
	bool is_finished;            // Завершен ли таймер
	uint64_t start_time;         // Время начала
	
	// Параметры текста
	char *font_name;
	int font_size;
	uint32_t color;
	
	// Эффекты
	float fade_in_duration;      // Продолжительность появления (секунды)
	float fade_out_duration;     // Продолжительность затухания (секунды)
	float current_alpha;         // Текущая прозрачность
	
	// Действия по завершении
	enum finish_action {
		ACTION_NONE,
		ACTION_HIDE,
		ACTION_RESET,
		ACTION_SHOW_MESSAGE
	} action_on_finish;
	
	char *finish_message;
	
	// Для рендеринга
	gs_texture_t *texture;
	uint32_t cx;
	uint32_t cy;
};

// Форматирование времени MM:SS
static std::string format_time(int seconds)
{
	if (seconds < 0) seconds = 0;
	int minutes = seconds / 60;
	int secs = seconds % 60;
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, secs);
	return std::string(buffer);
}

// Получение имени источника
static const char *timer_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("TimerSource");
}

// Создание источника
static void *timer_create(obs_data_t *settings, obs_source_t *source)
{
	struct timer_source *context = (struct timer_source *)bzalloc(sizeof(struct timer_source));
	context->source = source;
	context->is_running = false;
	context->is_finished = false;
	context->current_alpha = 0.0f;
	
	// Загрузка настроек (будет выполнено в timer_update)
	context->font_name = nullptr;
	context->finish_message = nullptr;
	
	return context;
}

// Удаление источника
static void timer_destroy(void *data)
{
	struct timer_source *context = (struct timer_source *)data;
	
	if (context->font_name)
		bfree(context->font_name);
	if (context->finish_message)
		bfree(context->finish_message);
	if (context->texture)
		gs_texture_destroy(context->texture);
	
	bfree(context);
}

// Обновление настроек
static void timer_update(void *data, obs_data_t *settings)
{
	struct timer_source *context = (struct timer_source *)data;
	
	// Обновляем параметры таймера
	context->duration_seconds = (int)obs_data_get_int(settings, "duration");
	if (!context->is_running) {
		context->remaining_seconds = context->duration_seconds;
	}
	
	// Обновляем параметры шрифта
	const char *font = obs_data_get_string(settings, "font");
	if (context->font_name)
		bfree(context->font_name);
	context->font_name = bstrdup(font);
	context->font_size = (int)obs_data_get_int(settings, "font_size");
	
	// Обновляем цвет
	context->color = (uint32_t)obs_data_get_int(settings, "color");
	
	// Обновляем эффекты
	context->fade_in_duration = (float)obs_data_get_double(settings, "fade_in");
	context->fade_out_duration = (float)obs_data_get_double(settings, "fade_out");
	
	// Обновляем действие по завершении
	const char *action = obs_data_get_string(settings, "finish_action");
	if (strcmp(action, "hide") == 0)
		context->action_on_finish = timer_source::ACTION_HIDE;
	else if (strcmp(action, "reset") == 0)
		context->action_on_finish = timer_source::ACTION_RESET;
	else if (strcmp(action, "show_message") == 0)
		context->action_on_finish = timer_source::ACTION_SHOW_MESSAGE;
	else
		context->action_on_finish = timer_source::ACTION_NONE;
	
	const char *msg = obs_data_get_string(settings, "finish_message");
	if (context->finish_message)
		bfree(context->finish_message);
	context->finish_message = bstrdup(msg);
}

// Получение свойств
static obs_properties_t *timer_properties(void *data)
{
	UNUSED_PARAMETER(data);
	
	obs_properties_t *props = obs_properties_create();
	
	// Длительность таймера
	obs_properties_add_int(props, "duration", 
		obs_module_text("Duration"), 1, 7200, 1);
	
	// Настройки шрифта
	obs_properties_add_text(props, "font", 
		obs_module_text("Font"), OBS_TEXT_DEFAULT);
	obs_properties_add_int(props, "font_size", 
		obs_module_text("FontSize"), 12, 200, 1);
	obs_properties_add_color(props, "color", 
		obs_module_text("Color"));
	
	// Эффекты
	obs_properties_add_float(props, "fade_in", 
		obs_module_text("FadeIn"), 0.0, 10.0, 0.1);
	obs_properties_add_float(props, "fade_out", 
		obs_module_text("FadeOut"), 0.0, 10.0, 0.1);
	
	// Действие по завершении
	obs_property_t *action = obs_properties_add_list(props, "finish_action",
		obs_module_text("FinishAction"), OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(action, obs_module_text("ActionNone"), "none");
	obs_property_list_add_string(action, obs_module_text("ActionHide"), "hide");
	obs_property_list_add_string(action, obs_module_text("ActionReset"), "reset");
	obs_property_list_add_string(action, obs_module_text("ActionShowMessage"), "show_message");
	
	obs_properties_add_text(props, "finish_message", 
		obs_module_text("FinishMessage"), OBS_TEXT_DEFAULT);
	
	// Кнопки управления
	obs_properties_add_button(props, "start_button", 
		obs_module_text("Start"), nullptr);
	obs_properties_add_button(props, "pause_button", 
		obs_module_text("Pause"), nullptr);
	obs_properties_add_button(props, "reset_button", 
		obs_module_text("Reset"), nullptr);
	
	return props;
}

// Получение значений по умолчанию
static void timer_defaults(obs_data_t *settings)
{
	obs_data_set_default_int(settings, "duration", 60);
	obs_data_set_default_string(settings, "font", "Arial");
	obs_data_set_default_int(settings, "font_size", 72);
	obs_data_set_default_int(settings, "color", 0xFFFFFFFF);
	obs_data_set_default_double(settings, "fade_in", 0.5);
	obs_data_set_default_double(settings, "fade_out", 0.5);
	obs_data_set_default_string(settings, "finish_action", "none");
	obs_data_set_default_string(settings, "finish_message", "TIME'S UP!");
}

// Обновление таймера
static void timer_tick(void *data, float seconds)
{
	struct timer_source *context = (struct timer_source *)data;
	
	if (!context->is_running || context->is_finished)
		return;
	
	// Обновляем оставшееся время
	uint64_t current_time = os_gettime_ns();
	uint64_t elapsed = current_time - context->start_time;
	int elapsed_seconds = (int)(elapsed / 1000000000ULL);
	
	context->remaining_seconds = context->duration_seconds - elapsed_seconds;
	
	// Проверяем завершение
	if (context->remaining_seconds <= 0) {
		context->remaining_seconds = 0;
		context->is_finished = true;
		context->is_running = false;
		
		// Выполняем действие
		switch (context->action_on_finish) {
		case timer_source::ACTION_HIDE:
			obs_source_set_enabled(context->source, false);
			break;
		case timer_source::ACTION_RESET:
			context->remaining_seconds = context->duration_seconds;
			context->is_finished = false;
			break;
		case timer_source::ACTION_SHOW_MESSAGE:
			// Сообщение уже будет показано при рендеринге
			break;
		default:
			break;
		}
	}
	
	// Обновляем эффект появления/затухания
	float total_time = (float)context->duration_seconds;
	float elapsed_f = (float)elapsed_seconds;
	
	if (context->fade_in_duration > 0 && elapsed_f < context->fade_in_duration) {
		context->current_alpha = elapsed_f / context->fade_in_duration;
	} else if (context->fade_out_duration > 0 && 
	           (context->remaining_seconds) < context->fade_out_duration) {
		context->current_alpha = context->remaining_seconds / context->fade_out_duration;
	} else {
		context->current_alpha = 1.0f;
	}
}

// Получение размеров
static uint32_t timer_get_width(void *data)
{
	struct timer_source *context = (struct timer_source *)data;
	return context->cx;
}

static uint32_t timer_get_height(void *data)
{
	struct timer_source *context = (struct timer_source *)data;
	return context->cy;
}

// Рендеринг
static void timer_render(void *data, gs_effect_t *effect)
{
	struct timer_source *context = (struct timer_source *)data;
	
	if (!context->texture)
		return;
	
	// Применяем прозрачность
	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), 
	                      context->texture);
	
	gs_blend_state_push();
	gs_blend_function(GS_BLEND_SRCALPHA, GS_BLEND_INVSRCALPHA);
	
	gs_draw_sprite(context->texture, 0, context->cx, context->cy);
	
	gs_blend_state_pop();
}

// Обновление видео тика (для создания текстуры)
static void timer_video_tick(void *data, float seconds)
{
	struct timer_source *context = (struct timer_source *)data;
	
	// Здесь должна быть логика создания текстуры с текстом
	// Для упрощения, текстура создается как заглушка
	// В реальной реализации нужно использовать FreeType или GDI+ для отрисовки текста
	
	std::string time_str;
	if (context->is_finished && context->action_on_finish == timer_source::ACTION_SHOW_MESSAGE) {
		time_str = context->finish_message ? context->finish_message : "DONE!";
	} else {
		time_str = format_time(context->remaining_seconds);
	}
	
	// Примерный размер текстуры
	context->cx = context->font_size * 6;
	context->cy = context->font_size * 2;
}

// Начать таймер (функция для вызова извне)
static void timer_start(struct timer_source *context)
{
	if (!context->is_running && !context->is_finished) {
		context->is_running = true;
		context->start_time = os_gettime_ns();
	} else if (context->is_finished) {
		// Если завершен, сбрасываем и начинаем заново
		context->remaining_seconds = context->duration_seconds;
		context->is_finished = false;
		context->is_running = true;
		context->start_time = os_gettime_ns();
	}
}

// Структура информации об источнике
struct obs_source_info timer_source_info = {
	.id = "timer_source",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW,
	.get_name = timer_get_name,
	.create = timer_create,
	.destroy = timer_destroy,
	.get_defaults = timer_defaults,
	.get_properties = timer_properties,
	.update = timer_update,
	.video_tick = timer_video_tick,
	.video_render = timer_render,
	.get_width = timer_get_width,
	.get_height = timer_get_height,
};

// Инициализация модуля
bool obs_module_load(void)
{
	obs_register_source(&timer_source_info);
	return true;
}
