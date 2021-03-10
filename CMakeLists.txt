cmake_minimum_required(VERSION 3.10)

# set the project name
project(ModbusTest)

# add the executable
add_executable(ModbusTest test.cpp)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

target_link_libraries(ModbusTest ${LIBMODBUS_LIBRARIES})
target_include_directories(ModbusTest PUBLIC ${LIBMODBUS_INCLUDE_DIRS})
target_compile_options(ModbusTest PUBLIC ${LIBMODBUS_CFLAGS_OTHER})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


