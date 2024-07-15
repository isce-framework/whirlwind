# CMake>=3.25 is required for `add_subdirectory(... SYSTEM)`, which is convenient for
# working with Git submodules. The maximum version may be updated occasionally but
# should not exceed the latest CMake version tested by the CI system.
cmake_minimum_required(VERSION 3.25...3.29)

if(NOT DEFINED SKBUILD_PROJECT_NAME)
  message(
    FATAL_ERROR
      "This project is intended to be installed as a Python extension module using the"
      " scikit-build-core build backend. Standalone CMake builds are not supported."
  )
endif()

project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

# By default in CMake>=3.28, CMake will scan C++ sources of each target with C++20 or
# higher enabled for module dependencies. With Clang, this requires the
# `clang-scan-deps` tool, which may not be included in some Clang distributions. We're
# not using modules so disable the scan.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Add submodule project(s).
add_subdirectory(ext SYSTEM)

# Search for the Python components required by nanobind, as well as nanobind itself.
find_package(
  Python
  COMPONENTS Interpreter Development.Module
  REQUIRED
)
find_package(nanobind CONFIG REQUIRED)

add_subdirectory(src/whirlwind/graph/_lib)
add_subdirectory(src/whirlwind/network/_lib)
add_subdirectory(src/whirlwind/spline/_lib)
add_subdirectory(src/whirlwind/_lib)
