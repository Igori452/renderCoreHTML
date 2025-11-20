# Этап 1: Базовый образ (минимальный)
FROM gcc:12.2.0 AS base
RUN apt-get update && apt-get install -y \
    cmake make build-essential ca-certificates \
    # X11, VNC и оконный менеджер
    xvfb \
    x11vnc \
    x11-utils \
    fluxbox \
    # Дополнительные утилиты
    wget git python3-numpy \
    # noVNC зависимости
    websockify \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /workspace

# Устанавливаем noVNC
RUN git clone https://github.com/novnc/noVNC.git /opt/novnc \
    && git clone https://github.com/novnc/websockify /opt/novnc/utils/websockify \
    && ln -s /opt/novnc/vnc.html /opt/novnc/index.html

# Этап 2: Gumbo
FROM base AS gumbo
RUN apt-get update && apt-get install -y \
    wget autoconf automake libtool \
    && rm -rf /var/lib/apt/lists/*
RUN wget -q https://github.com/google/gumbo-parser/archive/refs/tags/v0.10.1.tar.gz -O - | tar -xz \
    && cd gumbo-parser-0.10.1 \
    && ./autogen.sh && ./configure && make -j$(nproc) \
    && cp src/*.h /usr/local/include/ \
    && make install \
    && cd .. \
    && rm -rf gumbo-parser-0.10.1

# Этап 3: SFML  
FROM base AS sfml
RUN apt-get update && apt-get install -y \
    libsfml-dev \
    # Графические зависимости для SFML
    libgl1-mesa-dev \
    libxrandr-dev \
    libx11-dev \
    libxcb1-dev \
    libudev-dev \
    libfreetype6-dev \
    libflac-dev \
    libvorbis-dev \
    libopenal-dev \
    && rm -rf /var/lib/apt/lists/*

# Этап 4: Финальный образ
FROM base AS full
COPY --from=gumbo /usr/local/lib/libgumbo.so.1 /usr/local/lib/
COPY --from=gumbo /usr/local/include/ /usr/local/include/
COPY --from=sfml /usr/lib/x86_64-linux-gnu/libsfml-* /usr/lib/x86_64-linux-gnu/
COPY --from=sfml /usr/lib/x86_64-linux-gnu/libGL.so.1 /usr/lib/x86_64-linux-gnu/
COPY --from=sfml /usr/lib/x86_64-linux-gnu/libXrandr.so.2 /usr/lib/x86_64-linux-gnu/
COPY --from=sfml /usr/lib/x86_64-linux-gnu/libX11.so.6 /usr/lib/x86_64-linux-gnu/
COPY --from=sfml /usr/include/SFML/ /usr/include/SFML/
RUN ldconfig

# Установка скрипта запуска VNC
COPY start-vnc.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/start-vnc.sh

# Запуск bash по умолчанию для разработки
CMD ["/bin/bash"]

# Варианты тегов
FROM base AS latest
FROM gumbo AS gumbo-only
FROM sfml AS sfml-only
FROM full AS full-version