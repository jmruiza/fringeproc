# This is a CMake example for Python
cmake_minimum_required(VERSION 2.8)
project(fringeproc)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -std=gnu99 -O2  -funroll-loops")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-fPIC -std=gnu99 -ggdb3 -DBZ_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -ggdb3 -DBZ_DEBUG")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Look for required libraries and tools
find_package(OpenCV REQUIRED)
find_package(SWIG REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(NumPy REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
include(${SWIG_USE_FILE})

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
add_subdirectory(python)
