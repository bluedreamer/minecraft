cmake_minimum_required(VERSION 3.14)
project(minecraft)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(cpp/nbt)
include_directories(cpp/save_files)

add_library(nbt
        cpp/nbt/Tag.cpp cpp/nbt/Tag.h cpp/nbt_reader.cpp cpp/nbt/Reader.cpp cpp/nbt/Reader.h)

add_executable(level_dat_reader cpp/save_files/Level.cpp cpp/save_files/Level.h cpp/level_dat_reader.cpp
        cpp/save_files/DataPacks.cpp cpp/save_files/DataPacks.h cpp/save_files/DimensionData.cpp
        cpp/save_files/DimensionData.h cpp/save_files/GameRules.cpp cpp/save_files/GameRules.h
        cpp/save_files/GeneratorOptions.cpp cpp/save_files/GeneratorOptions.h cpp/save_files/Player.cpp
        cpp/save_files/Player.h cpp/save_files/Version.cpp cpp/save_files/Version.h cpp/save_files/CustomBossEvents.cpp
        cpp/save_files/CustomBossEvents.h)
target_link_libraries(level_dat_reader nbt boost_iostreams)