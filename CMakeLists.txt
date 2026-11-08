# Require a modern CMake version for native CUDA support
cmake_minimum_required(VERSION 3.18)

# Enable both CXX (C++) and CUDA languages
project(llm_engine LANGUAGES CXX CUDA)

# Enforce C++ 23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimize for speed (-O3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O3")

# Tell CMake where to find our header files
include_directories(include)
include_directories(third_party)

# Find the CUDA Toolkit and cuBLAS library
find_package(CUDAToolkit REQUIRED)

# Gather all source files
file(GLOB SOURCES "src/*.cpp" "src/*.cu")

# Create the executable
add_executable(llm_engine ${SOURCES})

# Link the cuBLAS library to our executable
target_link_libraries(llm_engine PRIVATE CUDA::cublas CUDA::cudart)