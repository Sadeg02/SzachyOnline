cmake_minimum_required(VERSION 3.26)
project(SzachyOnline)

set(CMAKE_CXX_STANDARD 17)
enable_testing()
add_executable(SzachyOnline
        klient.c
        serwer.c
        komendy.c
        szachownica.c
        szachownica.h
        komendy.h
        test.c
)
enable_testing()

