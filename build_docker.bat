@echo off
REM Быстрая сборка через Docker

echo ========================================
echo OBS Timer Plugin - Docker Build
echo ========================================
echo.

REM Проверка Docker
where docker >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Docker не установлен.
    echo.
    echo Установите Docker Desktop:
    echo   https://www.docker.com/products/docker-desktop
    echo.
    echo Или используйте GitHub Actions для облачной сборки:
    echo   Смотрите BUILD_CLOUD.md
    echo.
    pause
    exit /b 1
)

echo Сборка плагина через Docker (займет 5-10 минут при первом запуске)...
echo.

REM Сборка образа и компиляция
docker build -t obs-timer-plugin .
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Ошибка сборки в Docker
    pause
    exit /b 1
)

REM Создание release директории
if not exist "release" mkdir release

REM Извлечение скомпилированного плагина из контейнера
echo.
echo Извлечение готового плагина...
docker create --name temp-container obs-timer-plugin
docker cp temp-container:/plugin/build/obs-timer-plugin.so release/
docker rm temp-container

echo.
echo ========================================
echo Сборка завершена!
echo ========================================
echo.
echo Плагин находится в: release\obs-timer-plugin.so
echo.
echo ВНИМАНИЕ: Это Linux версия (.so)
echo Для Windows нужна облачная сборка через GitHub Actions
echo Смотрите: BUILD_CLOUD.md
echo.
pause
