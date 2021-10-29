cmake_minimum_required (VERSION 3.18.4)
project (ImprovedShellBuiltins)

# find a boost install with the libraries filesystem and system
find_package(Boost 1.74 REQUIRED COMPONENTS filesystem system program_options log log_setup)
# Create a sources variable with a link to all cpp files to compile

# check if boost was found
if(Boost_FOUND)
    message ("boost found")
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()

# try conditional compilation
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
CHECK_CXX_COMPILER_FLAG("-std=c++2a" COMPILER_SUPPORTS_CXX2A)

# check results and add flag
if(COMPILER_SUPPORTS_CXX20)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
elseif(COMPILER_SUPPORTS_CXX2A)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler.")
endif()

# Add sub directories
add_subdirectory(mkdir)
