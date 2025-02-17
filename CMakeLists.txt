cmake_minimum_required(VERSION 3.29)
project(XilinxCode C)

set(CMAKE_C_STANDARD 23)

add_executable(XilinxCode main.c
		helper.c
		writing.c)

project (Testing C)
add_executable(Testing tests.c
		helper.c
		writing.c
)
