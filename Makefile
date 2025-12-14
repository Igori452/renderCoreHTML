.PHONY: run build

# Сборка образа
build:
	docker build --target full -t mini-browser:full .

# Запуск контейнера с VNC и приложением
run:
	docker run -it --rm -p 8080:8080 -v ${PWD}:/workspace mini-browser:full 