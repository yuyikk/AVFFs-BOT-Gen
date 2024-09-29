cmake_minimum_required(VERSION 3.10)

# Project name
project(eP_Generator)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the ROOT package
find_package(ROOT REQUIRED COMPONENTS RIO Net Hist Tree)

# Set up ROOT
include(${ROOT_USE_FILE})

# Your custom class headers and sources
set(SOURCES ${PROJECT_SOURCE_DIR}/include/*.h)  # Replace with your source files
set(HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)  # Replace with your header files

# Create the executable
add_executable(esepp esepp.cxx esepp.h ${SOURCES})

# Link against ROOT libraries
target_link_libraries(esepp ${ROOT_LIBRARIES})

# Include directories
target_include_directories(esepp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

# Set additional compile options if needed
# target_compile_options(myrootapp PRIVATE -Wall -Wextra)