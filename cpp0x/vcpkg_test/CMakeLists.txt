cmake_minimum_required (VERSION 3.12)

if(NOT DEFINED $ENV{VCPKG_ROOT})
  set(VCPKG_ROOT $ENV{HOMEDRIVE}$ENV{HOMEPATH}/vcpkg_cpptruths)
endif()

if(NOT EXISTS ${VCPKG_ROOT})
  message("Cloning vcpkg in ${VCPKG_ROOT}")
  execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${VCPKG_ROOT})
endif()

if(NOT EXISTS ${VCPKG_ROOT}/README.md)
  message(FATAL_ERROR "***** FATAL ERROR: Could not clone vcpkg *****")
endif()

if(NOT EXISTS ${VCPKG_ROOT}/vcpkg.exe)
  message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
  execute_process(COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.bat WORKING_DIRECTORY ${VCPKG_ROOT})
endif()

if(NOT EXISTS ${VCPKG_ROOT}/vcpkg.exe)
  message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
endif()

set(MY_PROJECT_DEPENDENCIES boost-core boost-optional)
message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
execute_process(COMMAND ${VCPKG_ROOT}/vcpkg.exe install ${MY_PROJECT_DEPENDENCIES} WORKING_DIRECTORY ${VCPKG_ROOT})
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
#set(Boost_INCLUDE_DIR ${VCPKG_ROOT}/installed/x86-windows/include)
project (CppGenerators CXX)
find_package(Boost 1.67 REQUIRED)

message("***** BOOST INCLUDE DIRS: ${Boost_INCLUDE_DIR} *****")

include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})
add_executable(driver test/driver.cxx)
set_property(TARGET driver PROPERTY CXX_STANDARD 17)
