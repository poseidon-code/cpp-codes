cmake_minimum_required(VERSION 3.25)
project(Serial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIBSERIAL_INCLUDE_DIRECTORIES include)
set(LIBSERIAL_SOURCES src/Serial.cpp)


add_executable(SerialTransmitter SerialTransmitter.cpp ${LIBSERIAL_SOURCES})
target_include_directories(SerialTransmitter PUBLIC ${LIBSERIAL_INCLUDE_DIRECTORIES})

add_executable(SerialReceiver SerialReceiver.cpp ${LIBSERIAL_SOURCES})
target_include_directories(SerialReceiver PUBLIC ${LIBSERIAL_INCLUDE_DIRECTORIES})
