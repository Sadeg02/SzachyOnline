cmake_minimum_required(VERSION 3.26)
project(SzachyOnline)

set(CMAKE_CXX_STANDARD 17)

add_executable(SzachyOnline main.cpp
        figury.h
        ruchy.cpp
        plansza.cpp
        plansza.h
        testy.cpp)
