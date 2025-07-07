
# About the Files 

Use make file to build `./benchmark`, `./baseline`, `./final`

`./final` is produced by makefile by just running `make` 
Others can be compiled like normal cpp files

## `benchmark.cpp`
You can use benchmark.cpp to compare the times directly. It uses chrono library to time how much a process takes t complete starting from forking and exec'ing

## `baseline.cpp`
Contains the default memory manager provided by the system 

## `mem_manage.hpp`, `mem_manage.cpp`
Contain the implementation of the custom memory allocator


