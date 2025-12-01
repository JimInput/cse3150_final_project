# Require CMake 3.15+ (matching scikit-build-core) Use new versions of all
# policies up to CMake 4.0
cmake_minimum_required(VERSION 3.15...4.0)

# Scikit-build-core sets these values for you, or you can just hard-code the
# name and version.
project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

# --- Request C++17 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# ---------------------

# Find pybind11 and Python
find_package(pybind11 CONFIG REQUIRED)

# Add a library using FindPython's tooling (pybind11 also provides a helper like
# this)
python_add_library(_core MODULE
  src/cpp/main.cpp
  src/cpp/src/node.cpp
  src/cpp/src/graph.cpp
  src/cpp/src/announcement.cpp
  src/cpp/src/bgp.cpp
  src/cpp/src/file_reader.cpp
  src/cpp/src/file_writer.cpp
  WITH_SOABI)

# Add include directory for headers
target_include_directories(_core PRIVATE src/cpp/include)

target_link_libraries(_core PRIVATE pybind11::headers)

# This is passing in the version as a define just as an example
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# The install directory is the output (wheel) directory
install(TARGETS _core DESTINATION cse3150_final_project)
