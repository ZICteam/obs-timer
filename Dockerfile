# syntax=docker/dockerfile:1
FROM ubuntu:22.04

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libobs-dev \
    obs-studio \
    && rm -rf /var/lib/apt/lists/*

# Рабочая директория
WORKDIR /plugin

# Копирование исходников
COPY . .

# Сборка
RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Результат будет в /plugin/build/obs-timer-plugin.so
CMD ["bash"]
