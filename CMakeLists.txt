cmake_minimum_required(VERSION 3.27.6)

# Устанавливаем имя проекта
project(MyProject)

# Включить поддержку C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавить исполняемый файл main.cpp
add_executable(MyProject src/main.cpp)

# Включить поддержку тестирования с gtest и gmock
include(FetchContent)
FetchContent_Declare(
  googletest
  # Укажите версию или коммит, который вам нужен
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

# Запретить переопределение настроек компилятора/линкера родительского проекта для Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Загрузить и сделать доступной библиотеку gtest и gmock
FetchContent_MakeAvailable(googletest)

# Создать тесты
add_executable(MyProjectTests src/test.cpp)

# Связать тесты с gtest и gmock
target_link_libraries(MyProjectTests gtest gmock gtest_main)

# Добавить зависимость от вашего проекта и библиотеки, если они существуют
# target_link_libraries(MyProjectTests MyProject YourLibrary)

# Добавить тесты в CTest
include(GoogleTest)
gtest_discover_tests(MyProjectTests)
