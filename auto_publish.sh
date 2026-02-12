#!/bin/bash
# Автоматический скрипт для загрузки проекта на GitHub

echo "========================================"
echo "Автоматическая загрузка на GitHub"
echo "========================================"
echo ""

# Проверка Git
if ! command -v git &> /dev/null; then
    echo "ERROR: Git не установлен!"
    exit 1
fi

echo "[1/5] Проверка Git репозитория..."
if [ ! -d ".git" ]; then
    echo "Инициализация Git..."
    git init
    git config user.name "OBS Plugin Developer"
    git config user.email "dev@example.com"
    git add .
    git commit -m "OBS Timer Plugin v1.0.0"
fi

echo ""
echo "[2/5] Настройка удаленного репозитория..."
echo ""
read -p "Введите ваш GitHub username: " GITHUB_USER
read -p "Введите название репозитория (по умолчанию: obs-timer-plugin): " REPO_NAME
REPO_NAME=${REPO_NAME:-obs-timer-plugin}

echo ""
echo "[3/5] Добавление remote origin..."
git remote remove origin 2>/dev/null
git remote add origin "https://github.com/$GITHUB_USER/$REPO_NAME.git"

echo ""
echo "[4/5] Отправка кода на GitHub..."
echo "ВНИМАНИЕ: Потребуется ваш логин и пароль/token GitHub"
echo ""

git branch -M main
git push -u origin main

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Не удалось отправить код на GitHub"
    echo ""
    echo "Проверьте:"
    echo "1. Создан ли репозиторий $REPO_NAME на GitHub"
    echo "2. Правильно ли введен username"
    echo "3. Есть ли доступ к интернету"
    echo ""
    echo "Создайте репозиторий: https://github.com/new"
    exit 1
fi

echo ""
echo "[5/5] Готово!"
echo ""
echo "========================================"
echo "Код успешно загружен на GitHub!"
echo "========================================"
echo ""
echo "Теперь запустите автоматическую сборку:"
echo ""
echo "1. Откройте: https://github.com/$GITHUB_USER/$REPO_NAME/actions"
echo "2. Нажмите: 'I understand my workflows, go ahead and enable them'"
echo "3. Выберите: 'Build OBS Timer Plugin'"
echo "4. Нажмите: 'Run workflow'"
echo "5. Подождите 5-10 минут"
echo "6. Скачайте готовый плагин из Artifacts"
echo ""

# Попытка открыть браузер
if command -v xdg-open &> /dev/null; then
    xdg-open "https://github.com/$GITHUB_USER/$REPO_NAME/actions"
elif command -v open &> /dev/null; then
    open "https://github.com/$GITHUB_USER/$REPO_NAME/actions"
fi
