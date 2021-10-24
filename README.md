# CPPND: Capstone project Asteroids game

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

## Rubric point addressed
* Compiling and Testing
* Loops, Functions, I/O
* Object Oriented Programming
* Memory Management
  *
* Concurrency
  *

Prerequisites
--------------
* CMake 3.19.0 or later
* Windows
  * Environment variable `SDL2DIR` is set to extracted SDL2 development libraries directory e.g.
    `SDL2DIR=C:\Users\Public\Downloads\SDL2-2.0.16` 
    `Instructions: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php`
*Linux


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./AsteroidsGame`.

## Build Instructions on platforms
Windows
--------------

`cmake -G "Visual Studio 16 2019" -B.\build`
`cmake --build .\build`

* Build or build and install in single step
```
rmdir /q /s build & cmake -G "Visual Studio 16 2019" -B.\build & cmake --build .\build
rmdir /q /s build & cmake -G "Visual Studio 16 2019" -B.\build && cmake --build .\build && cmake --install .\build --config Debug
```
* Log to trace
```
rmdir /q /s build & cmake -G "Visual Studio 16 2019" --trace -H.\ -B.\build > trace.txt 2>&1 && cmake --build .\build
```

Linux
--------------
```
rm -rf build && cmake -Bbuild && cmake --build build

rm -rf build && cmake -Bbuild && cmake --build build && cmake --install build
```