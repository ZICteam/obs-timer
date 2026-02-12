# Установка плагина OBS Timer

## Быстрая установка (для пользователей)

### Windows

1. Скачайте последний релиз плагина (файл `obs-timer-plugin.dll`)

2. Найдите папку с установленным OBS Studio (обычно):
   ```
   C:\Program Files\obs-studio
   ```

3. Скопируйте `obs-timer-plugin.dll` в:
   ```
   C:\Program Files\obs-studio\obs-plugins\64bit\
   ```

4. Скопируйте папку `locale` из релиза в:
   ```
   C:\Program Files\obs-studio\data\obs-plugins\obs-timer-plugin\locale\
   ```
   
   Создайте папку `obs-timer-plugin`, если её нет.

5. Перезапустите OBS Studio

6. Плагин появится в списке источников как "Countdown Timer" или "Счётчик обратного отсчёта"

### Linux

```bash
# Скопируйте библиотеку плагина
sudo cp obs-timer-plugin.so /usr/lib/obs-plugins/

# Скопируйте файлы локализации
sudo mkdir -p /usr/share/obs/obs-plugins/obs-timer-plugin/
sudo cp -r locale /usr/share/obs/obs-plugins/obs-timer-plugin/

# Перезапустите OBS
```

### macOS

```bash
# Скопируйте плагин
cp obs-timer-plugin.so /Applications/OBS.app/Contents/PlugIns/

# Скопируйте файлы локализации
mkdir -p /Applications/OBS.app/Contents/Resources/obs-plugins/obs-timer-plugin/
cp -r locale /Applications/OBS.app/Contents/Resources/obs-plugins/obs-timer-plugin/

# Перезапустите OBS
```

## Сборка из исходников

Если вы хотите собрать плагин самостоятельно, смотрите инструкции в [README.md](README.md).

### Для Windows

Просто запустите `build.bat` и следуйте инструкциям.

## Проверка установки

1. Откройте OBS Studio
2. Нажмите "+" в панели источников
3. Найдите "Countdown Timer" в списке
4. Если источник присутствует - плагин установлен правильно!

## Устранение проблем

### Плагин не появляется в списке источников

1. Проверьте, что файл плагина находится в правильной папке
2. Проверьте, что у вас установлена 64-битная версия OBS Studio
3. Проверьте логи OBS: Справка → Файлы логов → Показать файлы логов
4. Убедитесь, что версия плагина совместима с вашей версией OBS

### OBS не запускается после установки

1. Удалите файл плагина из папки `obs-plugins`
2. Запустите OBS
3. Попробуйте использовать другую версию плагина

### Текст не отображается

1. Проверьте, что указанный шрифт установлен в системе
2. Попробуйте использовать стандартный шрифт (Arial, Times New Roman)
3. Увеличьте размер шрифта

## Удаление плагина

Просто удалите файлы плагина:

### Windows
```
C:\Program Files\obs-studio\obs-plugins\64bit\obs-timer-plugin.dll
C:\Program Files\obs-studio\data\obs-plugins\obs-timer-plugin\
```

### Linux
```bash
sudo rm /usr/lib/obs-plugins/obs-timer-plugin.so
sudo rm -rf /usr/share/obs/obs-plugins/obs-timer-plugin/
```

### macOS
```bash
rm /Applications/OBS.app/Contents/PlugIns/obs-timer-plugin.so
rm -rf /Applications/OBS.app/Contents/Resources/obs-plugins/obs-timer-plugin/
```
