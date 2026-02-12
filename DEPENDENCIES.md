# Установка зависимостей для сборки OBS Timer Plugin

Это руководство поможет вам установить все необходимые инструменты для сборки плагина.

## 🪟 Windows

### Метод 1: Автоматическая установка (рекомендуется)

Используя Windows Package Manager (winget):

```powershell
# 1. Установить CMake
winget install Kitware.CMake

# 2. Установить Visual Studio 2022 Community (с C++)
winget install Microsoft.VisualStudio.2022.Community --override "--add Microsoft.VisualStudio.Workload.NativeDesktop --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --includeRecommended --passive"

# 3. Установить Git (если еще не установлен)
winget install Git.Git

# 4. Перезапустить терминал
```

После установки перезапустите PowerShell или выполните:
```powershell
refreshenv
```

### Метод 2: Ручная установка

#### 1. CMake
**Вариант А: Через winget**
```powershell
winget install Kitware.CMake
```

**Вариант Б: Вручную**
1. Скачайте с https://cmake.org/download/
2. Выберите "Windows x64 Installer"
3. При установке отметьте "Add CMake to system PATH"

#### 2. Visual Studio 2022
**Вариант А: Через winget**
```powershell
winget install Microsoft.VisualStudio.2022.Community
```

**Вариант Б: Вручную**
1. Скачайте с https://visualstudio.microsoft.com/
2. Выберите "Visual Studio 2022 Community"
3. При установке выберите:
   - ✅ "Desktop development with C++"
   - ✅ "C++ CMake tools for Windows"

#### 3. OBS Studio (для разработки)

**Вариант А: Установленная версия**
```powershell
winget install OBSProject.OBSStudio
```
После установки SDK обычно находится в:
```
C:\Program Files\obs-studio\
```

**Вариант Б: Исходники** (для продвинутых разработчиков)
```powershell
git clone --recursive https://github.com/obsproject/obs-studio.git
cd obs-studio
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="C:\obs-build"
cmake --build . --config Release
cmake --install . --config Release
```

### Проверка установки

```powershell
# Проверить CMake
cmake --version

# Проверить компилятор
where cl

# Проверить Git
git --version
```

Все команды должны работать без ошибок.

---

## 🐧 Linux (Ubuntu/Debian)

### Установка зависимостей

```bash
# Обновить систему
sudo apt update

# Установить основные инструменты
sudo apt install build-essential cmake git

# Установить зависимости OBS
sudo apt install libobs-dev obs-studio

# Для Ubuntu 20.04/22.04 может потребоваться PPA:
sudo add-apt-repository ppa:obsproject/obs-studio
sudo apt update
sudo apt install obs-studio libobs-dev
```

### Проверка

```bash
cmake --version
gcc --version
pkg-config --modversion libobs
```

---

## 🍎 macOS

### Установка через Homebrew

```bash
# Установить Homebrew (если еще не установлен)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Установить зависимости
brew install cmake
brew install --cask obs

# Установить Xcode Command Line Tools
xcode-select --install
```

### Проверка

```bash
cmake --version
clang --version
```

---

## 🔧 Дополнительные инструменты (опционально)

### Visual Studio Code
Удобная IDE для разработки:

```powershell
# Windows
winget install Microsoft.VisualStudioCode

# Linux
sudo snap install code --classic

# macOS
brew install --cask visual-studio-code
```

**Рекомендуемые расширения:**
- C/C++ (Microsoft)
- CMake Tools (Microsoft)
- CMake Language Support

### Ninja Build System
Более быстрая альтернатива Make:

```powershell
# Windows
winget install Ninja-build.Ninja

# Linux
sudo apt install ninja-build

# macOS
brew install ninja
```

Использование:
```bash
cmake .. -G Ninja
ninja
```

### Ccache
Кэширование компиляции для ускорения пересборки:

```bash
# Linux
sudo apt install ccache

# macOS
brew install ccache

# Настройка
export CC="ccache gcc"
export CXX="ccache g++"
```

---

## 📦 Структура установки OBS

### Windows
```
C:\Program Files\obs-studio\
├── bin\                    # Исполняемые файлы
├── data\                   # Данные плагинов
├── obs-plugins\
│   └── 64bit\              # Плагины (.dll)
└── cmake\                  # CMake конфигурация (если есть)
```

### Linux
```
/usr/
├── lib/obs-plugins/        # Плагины (.so)
├── share/obs/              # Данные
└── include/obs/            # Заголовочные файлы
```

### macOS
```
/Applications/OBS.app/Contents/
├── PlugIns/                # Плагины
├── Resources/              # Данные
└── Resources/include/      # Заголовочные файлы (если есть)
```

---

## 🚀 Быстрый старт после установки

После установки всех зависимостей:

### Windows
```powershell
cd d:\OBS_Plugins\Timer
.\build.bat
```

### Linux/macOS
```bash
cd ~/OBS_Plugins/Timer
chmod +x build.sh
./build.sh
```

---

## ❓ Устранение проблем

### Windows: "CMake не найден"
```powershell
# Обновить PATH в текущей сессии
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Или перезапустить терминал
```

### Windows: "Visual Studio не найдена"
```powershell
# Установить Visual Studio Build Tools
winget install Microsoft.VisualStudio.2022.BuildTools

# Или использовать полную версию Community
winget install Microsoft.VisualStudio.2022.Community
```

### Linux: "libobs not found"
```bash
# Проверить установку
dpkg -l | grep obs

# Переустановить
sudo apt install --reinstall obs-studio libobs-dev
```

### macOS: "obs headers not found"
```bash
# OBS заголовки могут быть не установлены
# Нужно собрать OBS из исходников или указать путь вручную
brew install --HEAD obs
```

### CMake не находит OBS
Явно укажите путь:

```bash
# Windows
cmake .. -DCMAKE_PREFIX_PATH="C:/Program Files/obs-studio/cmake"

# Linux
cmake .. -DCMAKE_PREFIX_PATH="/usr/lib/cmake"

# macOS
cmake .. -DCMAKE_PREFIX_PATH="/Applications/OBS.app/Contents/Resources/cmake"
```

---

## 📚 Полезные ссылки

- [CMake Download](https://cmake.org/download/)
- [Visual Studio](https://visualstudio.microsoft.com/)
- [OBS Studio](https://obsproject.com/)
- [OBS Plugin Development](https://obsproject.com/docs/plugins.html)
- [CMake Documentation](https://cmake.org/documentation/)

---

## 💡 Совет

Создайте переменную окружения `OBS_SDK_PATH` для удобства:

### Windows (PowerShell)
```powershell
[System.Environment]::SetEnvironmentVariable('OBS_SDK_PATH', 'C:\Program Files\obs-studio', 'User')
```

### Linux/macOS (добавьте в ~/.bashrc или ~/.zshrc)
```bash
export OBS_SDK_PATH="/usr/local/obs"
```

Затем в CMake:
```bash
cmake .. -DCMAKE_PREFIX_PATH="$OBS_SDK_PATH/cmake"
```

---

**Нужна помощь?** Создайте [issue](https://github.com/yourusername/obs-timer-plugin/issues) с описанием проблемы.
