# Используем конкретную версию для стабильности
FROM ubuntu:22.04

# Отключаем интерактивные диалоги во время установки пакетов
ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем зависимости
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        gcc \
        gcovr \
        lcov \
        check \
        subunit \
        libsubunit-dev \
        pkg-config \
        git \
        make \
        && rm -rf /var/lib/apt/lists/*

# Указываем рабочую директорию
WORKDIR /app

# Сначала копируем только файлы сборки (если они есть), 
# чтобы лучше использовать кэш Docker. Но для C обычно копируют всё сразу:
COPY . .

# Собираем проект (лучше отделить сборку от тестирования)
# RUN make all

# По умолчанию запускаем тесты при старте контейнера, а не при сборке.
# Это позволит образу собраться, даже если в коде есть баги, 
# и ты сможешь зайти внутрь и разобраться.
CMD ["make", "clean", "test"]