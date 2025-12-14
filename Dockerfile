###########################
# Base image
###########################
FROM gcc:12.2.0 AS base

RUN apt-get update && apt-get install -y \
    cmake make build-essential ca-certificates \
    xvfb x11vnc x11-utils fluxbox \
    wget git python3-numpy \
    websockify \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# Install noVNC
RUN git clone https://github.com/novnc/noVNC.git /opt/novnc \
 && git clone https://github.com/novnc/websockify /opt/novnc/utils/websockify \
 && ln -s /opt/novnc/vnc.html /opt/novnc/index.html



###########################
# Gumbo Stage
###########################
FROM base AS gumbo

RUN apt-get update && apt-get install -y \
    wget autoconf automake libtool \
    && rm -rf /var/lib/apt/lists/*

RUN wget -q https://github.com/google/gumbo-parser/archive/refs/tags/v0.10.1.tar.gz -O - | tar -xz \
 && cd gumbo-parser-0.10.1 \
 && ./autogen.sh && ./configure && make -j$(nproc) \
 && cp src/*.h /usr/local/include/ \
 && make install



###########################
# SFML Stage
###########################
FROM base AS sfml

RUN apt-get update && apt-get install -y \
    libsfml-dev \
    libgl1-mesa-dev libxrandr-dev libx11-dev \
    libxcb1-dev libudev-dev libfreetype6-dev \
    libflac-dev libvorbis-dev libopenal-dev \
    && rm -rf /var/lib/apt/lists/*


###########################
# Final Image
###########################
FROM base AS full

# Bring Gumbo
COPY --from=gumbo /usr/local/lib/libgumbo.so.1 /usr/local/lib/
COPY --from=gumbo /usr/local/include/ /usr/local/include/

# Bring SFML
COPY --from=sfml /usr/lib/x86_64-linux-gnu/libsfml-* /usr/lib/x86_64-linux-gnu/
COPY --from=sfml /usr/include/SFML/ /usr/include/SFML/

RUN ldconfig

COPY start-vnc.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/start-vnc.sh

CMD ["/bin/bash"]
