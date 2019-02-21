cmake_minimum_required(VERSION 3.13)
project(wavetablesynth)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(waveSynthOld main.cpp WavetableOsc.cpp WavetableOsc.h)

add_executable(waveSynthEnum mainEnum.cpp WavetableOsc.cpp WavetableOsc.h)

add_executable(waveSynthBeta mainBeta.cpp WavetableOsc.cpp WavetableOsc.h)
