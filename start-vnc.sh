#!/bin/bash

# Автозапуск VNC + приложения
export DISPLAY=:0

# Переменные для PID процессов
XVFB_PID=""
VNC_PID=""
NOVNC_PID=""
APP_PID=""

# Функция для корректного завершения
cleanup() {
    echo ""
    echo "========================== Завершение работы ========================== "
    
    # Останавливаем процессы в правильном порядке
    [ -n "$APP_PID" ] && kill $APP_PID 2>/dev/null && sleep 1
    [ -n "$NOVNC_PID" ] && kill $NOVNC_PID 2>/dev/null && sleep 1
    [ -n "$VNC_PID" ] && kill $VNC_PID 2>/dev/null && sleep 1
    [ -n "$XVFB_PID" ] && kill $XVFB_PID 2>/dev/null
    
    echo "========================== Все процессы остановлены ========================== "
    exit 0
}

# Устанавливаем обработчик сигналов
trap cleanup SIGINT SIGTERM

echo "========================== Запуск Xvfb ========================== "
Xvfb $DISPLAY -screen 0 1920x1020x24 &
XVFB_PID=$!
sleep 2

echo "========================== Запуск Fluxbox ========================== "
fluxbox -display :0 &
sleep 1

echo "==========================  Запуск VNC сервера ========================== "
x11vnc -display $DISPLAY -forever -shared -nopw -listen 0.0.0.0 -rfbport 5900 &
VNC_PID=$!
sleep 2

echo "==========================  Запуск noVNC ========================== "
/opt/novnc/utils/novnc_proxy --vnc localhost:5900 --listen 8080 &
NOVNC_PID=$!
sleep 2

# Проверяем что VNC запустился
echo "==========================  Проверка запуска VNC ========================== "
if ! pgrep -x "x11vnc" > /dev/null; then
    echo "========================== VNC сервер не запустился ========================== "
    cleanup
fi

echo "--> VNC запущен: http://localhost:8080"


# Сборка и запуск приложения
echo "========================== Сборка приложения ========================== "
cd /workspace/build
if make -j4; then
    echo "========================== Запуск приложения ========================== "
    echo "========================== SFML window created successfully! ========================== "
    DISPLAY=:0 ./mini_browser &
    APP_PID=$!
    
    echo "** Для заврешения Ctr + C **"

    # Ждем завершения приложения
    wait $APP_PID
else
    echo "Ошибка сборки"
    cleanup
fi

# Если приложение завершилось, останавливаем VNC
echo "========================== Приложение завершено, останавливаем VNC ========================== "
cleanup