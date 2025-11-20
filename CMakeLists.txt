cmake_minimum_required(VERSION 3.10)
project(MiniBrowser)

# Отключаем тесты компилятора (говорим, о том что он уже проверен), которые создают конфликт
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER_WORKS 1)

# Указываем компилятору использовать C++17 стандарт.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для включения/выключения Gumbo
option(USE_GUMBO "Use Gumbo HTML parser" ON)

# Опция для включения/выключения SFML
option(USE_SFML "Use SFML for rendering" ON)

# Ищем все .cpp файлы рекурсивно
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Создаем исполняемый файл
add_executable(mini_browser ${SRC_FILES})

# Включаем все заголовочные файлы
target_include_directories(mini_browser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Запрещаем CMake добавлять тестовые файлы
set(CMAKE_CXX_COMPILER_ID_RUN 1)

# Условное подключение Gumbo
if(USE_GUMBO)
    # Принудительно указываем пути к Gumbo
    set(GUMBO_LIBRARY "/usr/local/lib/libgumbo.so.1")
    set(GUMBO_INCLUDE_DIR "/usr/local/include")
    
    if(EXISTS "${GUMBO_LIBRARY}" AND EXISTS "${GUMBO_INCLUDE_DIR}/gumbo.h")
        message(STATUS "  Gumbo found - HTML parsing enabled")
        message(STATUS "  Gumbo library: ${GUMBO_LIBRARY}")
        message(STATUS "  Gumbo include: ${GUMBO_INCLUDE_DIR}")
        
        # Подключаем Gumbo
        target_include_directories(mini_browser PRIVATE ${GUMBO_INCLUDE_DIR})
        target_link_libraries(mini_browser ${GUMBO_LIBRARY})
        target_compile_definitions(mini_browser PRIVATE USE_GUMBO_PARSER=1)
        
    else()
        message(WARNING "  Gumbo not found - HTML parsing disabled")
        message(WARNING "  GUMBO_LIBRARY: ${GUMBO_LIBRARY}")
        message(WARNING "  GUMBO_INCLUDE_DIR: ${GUMBO_INCLUDE_DIR}")
        target_compile_definitions(mini_browser PRIVATE USE_GUMBO_PARSER=0)
    endif()
else()
    message(STATUS "  Gumbo disabled by user")
    target_compile_definitions(mini_browser PRIVATE USE_GUMBO_PARSER=0)
endif()

# Условное подключение SFML
if(USE_SFML)
    # Ищем библиотеки SFML
    find_library(SFML_GRAPHICS_LIB sfml-graphics)
    find_library(SFML_WINDOW_LIB sfml-window)
    find_library(SFML_SYSTEM_LIB sfml-system)
    
    # Ищем заголовочные файлы SFML
    find_path(SFML_INCLUDE_DIR SFML/Graphics.hpp PATHS /usr/include /usr/local/include)
    
    if(SFML_GRAPHICS_LIB AND SFML_WINDOW_LIB AND SFML_SYSTEM_LIB AND SFML_INCLUDE_DIR)
        message(STATUS "  SFML found - rendering enabled")
        message(STATUS "  SFML graphics: ${SFML_GRAPHICS_LIB}")
        message(STATUS "  SFML window: ${SFML_WINDOW_LIB}")
        message(STATUS "  SFML system: ${SFML_SYSTEM_LIB}")
        message(STATUS "  SFML include: ${SFML_INCLUDE_DIR}")
        
        # Подключаем заголовки SFML
        target_include_directories(mini_browser PRIVATE ${SFML_INCLUDE_DIR})
        
        # Линкуем библиотеки SFML если нашли
        target_link_libraries(mini_browser 
            ${SFML_GRAPHICS_LIB}
            ${SFML_WINDOW_LIB}
            ${SFML_SYSTEM_LIB}
        )
        
        # Добавляем определение для кода
        target_compile_definitions(mini_browser PRIVATE USE_SFML_RENDERER=1)
        
    else()
        message(WARNING "  SFML not found - rendering disabled")
        message(WARNING "  SFML_GRAPHICS_LIB: ${SFML_GRAPHICS_LIB}")
        message(WARNING "  SFML_WINDOW_LIB: ${SFML_WINDOW_LIB}")
        message(WARNING "  SFML_SYSTEM_LIB: ${SFML_SYSTEM_LIB}")
        message(WARNING "  SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    endif()
else()
    message(STATUS "  SFML disabled by user")
    target_compile_definitions(mini_browser PRIVATE USE_SFML_RENDERER=0)
endif()