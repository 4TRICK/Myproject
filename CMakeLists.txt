cmake_minimum_required(VERSION 3.27.6)
project(MyProject)

add_executable (MyProject src/main.cpp)
#src/DataProxessorTest.cpp
# Укажите путь к каталогу с исходными кодами Google Test и Google Mock
#set(GTEST_SOURCE_DIR "C:/MinGW/Workspace/MyProject/testCmake/googletest/googletest")
#set(GMOCK_SOURCE_DIR "C:/MinGW/Workspace/MyProject/testCmake/googletest/googlemock")


# Добавьте подкаталоги с исходными кодами Google Test и Google Mock
#add_subdirectory(${GTEST_SOURCE_DIR})
#add_subdirectory(${GMOCK_SOURCE_DIR})

# Укажите, что вы хотите включить Google Test и Google Mock в ваш проект
#include_directories(${GTEST_SOURCE_DIR}/include ${GMOCK_SOURCE_DIR}/include)

# Свяжите Google Test и Google Mock с вашим исполняемым файлом
#target_link_libraries(MyProject gtest gtest_main gmock gmock_main)

# Укажите версию Google Test
#set(GTEST_VERSION "1.14.0")  # Замените на актуальную версию Google Test
