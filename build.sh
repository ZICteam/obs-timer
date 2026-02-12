#!/bin/bash

# Скрипт сборки плагина для Linux/macOS

echo "========================================"
echo "OBS Timer Plugin - Build Script"
echo "========================================"
echo ""

# Проверка наличия CMake
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake не найден. Установите CMake:"
    echo "  Ubuntu/Debian: sudo apt install cmake"
    echo "  Fedora: sudo dnf install cmake"
    echo "  macOS: brew install cmake"
    exit 1
fi

# Создание директории для сборки
mkdir -p build
cd build

echo "Конфигурирование проекта..."
echo ""

# Определение пути к OBS для разных ОС
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OBS_PATH="/usr/lib/cmake"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OBS_PATH="/Applications/OBS.app/Contents/Resources/cmake"
else
    echo "ERROR: Неподдерживаемая операционная система"
    exit 1
fi

# Конфигурация CMake
cmake .. -DCMAKE_PREFIX_PATH="$OBS_PATH"

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Ошибка конфигурации CMake."
    echo "Проверьте установку OBS Studio и путь к cmake файлам."
    cd ..
    exit 1
fi

echo ""
echo "Сборка проекта..."
echo ""

# Сборка
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Ошибка сборки проекта."
    cd ..
    exit 1
fi

echo ""
echo "========================================"
echo "Сборка завершена успешно!"
echo "========================================"
echo ""

# Информация об установке
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Для установки плагина выполните:"
    echo "  sudo make install"
    echo ""
    echo "Или скопируйте файлы вручную:"
    echo "  sudo cp libobs-timer-plugin.so /usr/lib/obs-plugins/"
    echo "  sudo cp -r ../data/locale /usr/share/obs/obs-plugins/obs-timer-plugin/"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Для установки плагина выполните:"
    echo "  sudo make install"
    echo ""
    echo "Или скопируйте файлы вручную:"
    echo "  cp libobs-timer-plugin.so /Applications/OBS.app/Contents/PlugIns/"
    echo "  mkdir -p /Applications/OBS.app/Contents/Resources/obs-plugins/obs-timer-plugin/"
    echo "  cp -r ../data/locale /Applications/OBS.app/Contents/Resources/obs-plugins/obs-timer-plugin/"
fi

echo ""

cd ..
