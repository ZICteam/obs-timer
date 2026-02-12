cmake_minimum_required(VERSION 3.16...3.26)

project(obs-timer-plugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение платформы
if(WIN32)
	set(OS_WINDOWS TRUE)
elseif(APPLE)
	set(OS_MACOS TRUE)
else()
	set(OS_LINUX TRUE)
endif()

# Поиск OBS Studio (необязательно для упрощённой сборки)
if(DEFINED ENV{OBS_SOURCE_DIR})
	set(OBS_INCLUDE_DIR "$ENV{OBS_SOURCE_DIR}/libobs")
	set(OBS_LIB_DIR "$ENV{OBS_SOURCE_DIR}/build/libobs/Release")
endif()

# Исходные файлы
set(PLUGIN_SOURCES
	src/timer-plugin.cpp
)

# Создание модуля плагина
add_library(obs-timer-plugin MODULE ${PLUGIN_SOURCES})

# Настройка включаемых директорий
if(OBS_INCLUDE_DIR)
	target_include_directories(obs-timer-plugin PRIVATE ${OBS_INCLUDE_DIR})
endif()

# Подключение библиотек OBS
if(OS_WINDOWS)
	if(OBS_LIB_DIR)
		if(EXISTS "${OBS_LIB_DIR}/obs.lib")
			target_link_libraries(obs-timer-plugin "${OBS_LIB_DIR}/obs.lib")
		elseif(EXISTS "${OBS_LIB_DIR}/../lib/obs.lib")
			target_link_libraries(obs-timer-plugin "${OBS_LIB_DIR}/../lib/obs.lib")
		endif()
	endif()
elseif(OS_LINUX)
	target_link_libraries(obs-timer-plugin obs)
endif()

# Установка свойств
set_target_properties(obs-timer-plugin PROPERTIES
	FOLDER "plugins"
	PREFIX ""
)

# Добавление файлов локализации
if(OS_WINDOWS)
	set(OBS_PLUGIN_DESTINATION "obs-plugins/64bit")
	set(OBS_DATA_DESTINATION "data/obs-plugins/obs-timer-plugin")
elseif(OS_MACOS)
	set(OBS_PLUGIN_DESTINATION "obs-plugins")
	set(OBS_DATA_DESTINATION "data/obs-plugins/obs-timer-plugin")
else()
	set(OBS_PLUGIN_DESTINATION "lib/obs-plugins")
	set(OBS_DATA_DESTINATION "share/obs/obs-plugins/obs-timer-plugin")
endif()

# Установка плагина
install(TARGETS obs-timer-plugin
	LIBRARY DESTINATION ${OBS_PLUGIN_DESTINATION}
	RUNTIME DESTINATION ${OBS_PLUGIN_DESTINATION}
)

# Установка данных локализации
install(DIRECTORY data/locale
	DESTINATION ${OBS_DATA_DESTINATION}
)

# Для Windows - копирование в директорию OBS
if(WIN32)
	# Можно указать путь к OBS Studio для автоматической установки
	# set(OBS_INSTALL_DIR "C:/Program Files/obs-studio")
	# if(EXISTS "${OBS_INSTALL_DIR}")
	#     add_custom_command(TARGET obs-timer-plugin POST_BUILD
	#         COMMAND ${CMAKE_COMMAND} -E copy
	#         $<TARGET_FILE:obs-timer-plugin>
	#         "${OBS_INSTALL_DIR}/obs-plugins/64bit/"
	#     )
	# endif()
endif()
