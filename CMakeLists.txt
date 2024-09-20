cmake_minimum_required(VERSION 3.29)
project(rayCastingDDA C)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(rayCastingDDA main.c)

target_link_libraries(rayCastingDDA
        OpenGL::GL
        GLUT::GLUT
        /usr/lib/x86_64-linux-gnu/libGLU.so  # Chemin direct vers GLU
        m
)
target_link_libraries(rayCastingDDA ${CURSES_LIBRARIES})