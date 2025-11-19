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
    find_library(GUMBO_LIBRARY gumbo)
    find_path(GUMBO_INCLUDE_DIR gumbo.h)
    
    if(GUMBO_LIBRARY AND GUMBO_INCLUDE_DIR)
        message(STATUS "✅ Gumbo found - HTML parsing enabled")
        
        # Подключаем заголовки Gumbo (добавление .h)
        target_include_directories(mini_browser PRIVATE ${GUMBO_INCLUDE_DIR})
        
        # Линкуем библиотеку Gumbo (реализация .h файлов в .cpp)
        target_link_libraries(mini_browser ${GUMBO_LIBRARY})
    else()
        message(WARNING "❌ Gumbo not found - HTML parsing disabled")
    endif()
else()
    message(STATUS "🔕 Gumbo disabled by user")
endif()