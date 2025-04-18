cmake_minimum_required(VERSION 3.30)

add_executable(game src/game/game.cpp src/game/game.hpp)
target_include_directories(game PRIVATE src/)
target_link_libraries(game PRIVATE neuron::neuron)

find_program(GLSLC_EXECUTABLE glslc)
file(GLOB_RECURSE SHADER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/src/*)

set(SHADER_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/src/)
set(SHADER_SPV_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/spv/)

function(add_shader source_file)
    cmake_path(ABSOLUTE_PATH source_file OUTPUT_VARIABLE source_abspath)
    cmake_path(RELATIVE_PATH source_abspath BASE_DIRECTORY ${SHADER_SOURCE_ROOT} OUTPUT_VARIABLE shader_relpath)
    cmake_path(ABSOLUTE_PATH shader_relpath BASE_DIRECTORY ${SHADER_SPV_ROOT} OUTPUT_VARIABLE shader_outpath)
    cmake_path(GET shader_relpath PARENT_PATH output_dir)
    set(shader_outpath "${shader_outpath}.spv")
    message("${source_file}  -  ${source_abspath}  -  ${shader_relpath}  -  ${shader_outpath}  -  ${output_dir}")
    
    file(MAKE_DIRECTORY ${output_dir})
    add_custom_command(OUTPUT "${shader_outpath}" COMMAND ${GLSLC_EXECUTABLE} "${source_abspath}" -o "${shader_outpath}" VERBATIM MAIN_DEPENDENCY "${shader_abspath}")
    return(PROPAGATE shader_outpath)
endfunction()

set(SHADER_OUTPUTS)

foreach(shader_file ${SHADER_SOURCES})
    add_shader(${shader_file})
    list(APPEND SHADER_OUTPUTS "${shader_outpath}")
endforeach()

add_custom_target(build_shaders DEPENDS ${SHADER_OUTPUTS})
add_dependencies(game build_shaders)
