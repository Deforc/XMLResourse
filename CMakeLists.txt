cmake_minimum_required(VERSION 3.26)
project(XMLResourse)

set(CMAKE_CXX_STANDARD 20)

add_executable(XMLResourse main.cpp
        Tree.cpp
        Tree.h
        Node.cpp
        Node.h
        XMLResourse.cpp
        XMLResourse.h
)
