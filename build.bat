@echo off
REM Скрипт сборки плагина для Windows

echo ========================================
echo OBS Timer Plugin - Build Script
echo ========================================
echo.

REM Проверка наличия CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake не найден!
    echo.
    echo Для установки CMake выполните:
    echo   winget install Kitware.CMake
    echo.
    echo После установки перезапустите терминал или выполните:
    echo   refreshenv
    echo.
    echo Или скачайте CMake с https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

REM Создание директории для сборки
if not exist "build" mkdir build
cd build

echo Конфигурирование проекта...
echo.

REM Замените путь ниже на путь к вашей установке OBS Studio
REM или к исходникам OBS Studio с cmake файлами
set OBS_PATH=C:\Program Files\obs-studio

REM Если у вас установлены исходники OBS, используйте путь к ним:
REM set OBS_PATH=C:\path\to\obs-studio\build

REM Автоматическое определение генератора Visual Studio
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%OBS_PATH%\cmake"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Ошибка конфигурации CMake.
    echo Проверьте путь к OBS Studio в переменной OBS_PATH.
    cd ..
    pause
    exit /b 1
)

echo.
echo Сборка проекта...
echo.

cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Ошибка сборки проекта.
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo Сборка завершена успешно!
echo ========================================
echo.
echo Скомпилированный плагин находится в: build\Release\
echo.
echo Для установки плагина:
echo 1. Скопируйте obs-timer-plugin.dll в:
echo    %ProgramFiles%\obs-studio\obs-plugins\64bit\
echo.
echo 2. Скопируйте папку data\locale в:
echo    %ProgramFiles%\obs-studio\data\obs-plugins\obs-timer-plugin\
echo.

cd ..
pause
