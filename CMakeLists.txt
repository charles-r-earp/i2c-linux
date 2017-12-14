cmake_minimum_required (VERSION 2.6)
project(i2c-linux)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

find_library(i2c_LIB i2c)

#message("${i2c_LIB}")

add_library(i2c-linux INTERFACE)
target_include_directories(i2c-linux INTERFACE include)

add_executable(test_servo examples/test_servo.cpp)
target_link_libraries(test_servo i2c-linux)

add_executable(test_motor examples/test_motor.cpp)
target_link_libraries(test_motor i2c-linux "${i2c_LIB}")

install(TARGETS i2c-linux LIBRARY DESTINATION lib)
install(FILES include/i2c_device.hpp include/pwm_driver.hpp DESTINATION include/i2c-linux)

