@echo off
REM Автоматический скрипт для загрузки проекта на GitHub и запуска сборки

echo ========================================
echo Автоматическая загрузка на GitHub
echo ========================================
echo.

REM Проверка Git
where git >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Git не установлен!
    echo Установите: winget install Git.Git
    pause
    exit /b 1
)

echo [1/5] Проверка Git репозитория...
if not exist ".git" (
    echo Инициализация Git...
    git init
    git config user.name "OBS Plugin Developer"
    git config user.email "dev@example.com"
    git add .
    git commit -m "OBS Timer Plugin v1.0.0"
)

echo.
echo [2/5] Настройка удаленного репозитория...
echo.
echo Введите ваш GitHub username:
set /p GITHUB_USER="> "

echo.
echo Введите название репозитория (по умолчанию: obs-timer-plugin):
set /p REPO_NAME="> "
if "%REPO_NAME%"=="" set REPO_NAME=obs-timer-plugin

echo.
echo [3/5] Добавление remote origin...
git remote remove origin 2>nul
git remote add origin https://github.com/%GITHUB_USER%/%REPO_NAME%.git

echo.
echo [4/5] Отправка кода на GitHub...
echo ВНИМАНИЕ: Потребуется ваш логин и пароль/token GitHub
echo.
pause

git branch -M main
git push -u origin main

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Не удалось отправить код на GitHub
    echo.
    echo Проверьте:
    echo 1. Создан ли репозиторий %REPO_NAME% на GitHub
    echo 2. Правильно ли введен username
    echo 3. Есть ли доступ к интернету
    echo.
    echo Создайте репозиторий: https://github.com/new
    echo Имя: %REPO_NAME%
    echo.
    pause
    exit /b 1
)

echo.
echo [5/5] Готово!
echo.
echo ========================================
echo Код успешно загружен на GitHub!
echo ========================================
echo.
echo Теперь запустите автоматическую сборку:
echo.
echo 1. Откройте: https://github.com/%GITHUB_USER%/%REPO_NAME%/actions
echo 2. Нажмите: "I understand my workflows, go ahead and enable them"
echo 3. Выберите: "Build OBS Timer Plugin"
echo 4. Нажмите: "Run workflow"
echo 5. Подождите 5-10 минут
echo 6. Скачайте готовый плагин из Artifacts
echo.
echo Или запустите:
echo    start https://github.com/%GITHUB_USER%/%REPO_NAME%/actions
echo.
pause

REM Открыть браузер
start https://github.com/%GITHUB_USER%/%REPO_NAME%/actions

echo.
echo Браузер открыт. Следуйте инструкциям выше.
echo.
pause
