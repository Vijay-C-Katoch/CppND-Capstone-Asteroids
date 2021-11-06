# CPPND: Capstone project ND Game Engine

## Rubric point addressed
* Compiling and Testing
* Loops, Functions, I/O
* Object Oriented Programming
* Memory Management
  *
* Concurrency
  *
  
# ND Game Engine : NanoDegree Game Engine  

## Create any 2D game using NND Game Engine

Prerequisites
--------------
* CMake 3.7 or later
* Windows
  * Environment variable `SDL2DIR` is set to extracted SDL2 development libraries directory e.g.
    `SDL2DIR=C:\Users\Public\Downloads\SDL2-2.0.16` 
  * Instructions: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php
  
* Linux
  * SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 


## Build Instructions on platforms
Windows 
--------------

`cmake -G "Visual Studio 16 2019" -B.\build`
`cmake --build .\build`

* Build or build and install in single step
```
rmdir /q /s build & cmake -G "Visual Studio 16 2019" -B.\build && cmake --build .\build && cmake --install .\build --config Debug
```

Linux
--------------
`rm -rf build && cmake -Bbuild && cmake --build build`

* Build or build and install in single step
`rm -rf build && cmake -Bbuild && cmake --build build && cmake --install build`


# License
* https://github.com/Vijay-C-Katoch/CppND-Capstone-Asteroids/blob/main/LICENSE.md