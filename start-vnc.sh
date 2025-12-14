#!/bin/bash

# Автозапуск VNC + приложения
export DISPLAY=:0

echo "*******************************************************"
echo "*               PROJECT STARTING...                   *"
echo "*******************************************************"

# Запуск всех сервисов в фоне
Xvfb $DISPLAY -screen 0 1920x1020x24 > /dev/null 2>&1 &
sleep 2

fluxbox -display :0 > /dev/null 2>&1 &
sleep 1

x11vnc -display $DISPLAY -forever -shared -nopw -listen 0.0.0.0 -rfbport 5900 > /dev/null 2>&1 &
sleep 2

/opt/novnc/utils/novnc_proxy --vnc localhost:5900 --listen 8080 > /dev/null 2>&1 &

echo "Сервисы запущены"
echo "Web: http://localhost:8080"
echo ""

# Сборка и запуск приложения
cd /workspace/build

if make -j4 > /dev/null 2>&1; then
    echo "Приложение собрано"
    echo "Запуск..."
    DISPLAY=:0 ./mini_browser
else
    echo "Ошибка сборки!"
    exit 1
fi