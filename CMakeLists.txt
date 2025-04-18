cmake_minimum_required(VERSION 3.30)
project(vulkaning)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(VulkanHeaders GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers/ GIT_TAG v1.4.312)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.15.2)
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG master CONFIGURE_COMMAND "" BUILD_COMMAND "")
FetchContent_Declare(VulkanMemoryAllocator GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git GIT_TAG v3.2.1)

set(SPDLOG_USE_STD_FORMAT ON)

FetchContent_MakeAvailable(VulkanHeaders glm glfw spdlog VulkanMemoryAllocator stb)

add_library(neuron src/neuron/neuron.cpp
        src/neuron/neuron.hpp
        src/neuron/window.cpp
        src/neuron/window.hpp
        src/neuron/render_system.cpp
        src/neuron/render_system.hpp
        src/neuron/app.cpp
        src/neuron/app.hpp
        src/neuron/render_target.cpp
        src/neuron/render_target.hpp
        src/neuron/command_recorder.cpp
        src/neuron/command_recorder.hpp
        src/neuron/dynamic_rendering_pass.cpp
        src/neuron/dynamic_rendering_pass.hpp
        src/neuron/primary_renderer.cpp
        src/neuron/primary_renderer.hpp
        src/neuron/pipeline.cpp
        src/neuron/pipeline.hpp)
target_include_directories(neuron PUBLIC src/ ${stb_SOURCE_DIR})
target_link_libraries(neuron PUBLIC Vulkan::Headers glm::glm glfw spdlog::spdlog GPUOpen::VulkanMemoryAllocator)
target_compile_definitions(neuron PUBLIC GLFW_INCLUDE_NONE GLFW_INCLUDE_VULKAN GLM_ENABLE_EXPERIMENTAL VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

add_library(neuron::neuron ALIAS neuron)

add_subdirectory(game)
