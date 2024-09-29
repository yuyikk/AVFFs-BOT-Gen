cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Sim_Neutron_E_Deposit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Geant4 REQUIRED ui_all vis_all)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/cmake)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# message(STATUS "ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}")
# message(STATUS "ROOT_LIBRARIES: ${ROOT_LIBRARIES}")

add_executable(doSimu doSimu.cc ${sources})

target_link_libraries(doSimu ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

################## Old Version ##################

# cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
# project(Sim_Neutron_E_Deposit)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# find_package(Geant4 REQUIRED ui_all vis_all)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/cmake)
# find_package(ROOT REQUIRED)


# include(${Geant4_USE_FILE})
# include_directories(${ROOT_INCLUDE_DIRS})
# include_directories(${PROJECT_SOURCE_DIR}/include)
# file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
# file(GLOB header ${PROJECT_SOURCE_DIR}/include/*.hh)
# message(STATUS "ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}")
# message(STATUS "ROOT_LIBRARIES: ${ROOT_LIBRARIES}")

# add_executable(doSimu doSimu.cc ${sources} ${header})
# target_link_libraries(doSimu ${Geant4_LIBRARIES})

# add_custom_target(Sim_Neutron_E_Deposit DEPENDS doSimu)

