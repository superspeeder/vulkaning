cmake_minimum_required(VERSION 3.30)

add_executable(game src/game/game.cpp src/game/game.hpp)
target_include_directories(game PRIVATE src/)
target_link_libraries(game PRIVATE neuron::neuron)
