cmake_minimum_required(VERSION 3.22)

project(TicketSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -fsanitize=undefined -fsanitize=leak -Wall -fno-omit-frame-pointer -O0")

include_directories(include)

add_executable(code
        src/utility.cpp
        src/User.cpp
        src/Train.cpp
        src/Order.cpp
        src/main.cpp
)