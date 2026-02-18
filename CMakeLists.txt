cmake_minimum_required(VERSION 4.1)
project(Solitaire)

set(CMAKE_CXX_STANDARD 20)

add_executable(Solitaire main.cpp
        Deck.h
        Deck.cpp
        Pyramid.h
        Pyramid.cpp)
