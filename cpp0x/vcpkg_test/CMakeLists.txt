cmake_minimum_required (VERSION 3.12)
set(MY_PROJECT_DEPENDENCIES boost-core boost-optional)

if(NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
  if(NOT DEFINED $ENV{VCPKG_ROOT})
    if(WIN32)
      set(VCPKG_ROOT $ENV{HOMEDRIVE}$ENV{HOMEPATH}/vcpkg_cpptruths)
    else()
      set(VCPKG_ROOT $ENV{HOME}/.vcpkg_cpptruths)
    endif()
  endif()

  if(NOT EXISTS ${VCPKG_ROOT})
    message("Cloning vcpkg in ${VCPKG_ROOT}")
    execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${VCPKG_ROOT})
    execute_process(COMMAND git checkout 52870c7595a63ade069ae51d5f4ee3a85fe4123f WORKING_DIRECTORY ${VCPKG_ROOT})
  endif()

  if(NOT EXISTS ${VCPKG_ROOT}/README.md)
    message(FATAL_ERROR "***** FATAL ERROR: Could not clone vcpkg *****")
  endif()

  if(WIN32)
    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
    set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
  else()
    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
    set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
  endif()

  if(NOT EXISTS ${VCPKG_EXEC})
    message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
    execute_process(COMMAND ${VCPKG_BOOTSTRAP} WORKING_DIRECTORY ${VCPKG_ROOT})
  endif()

  if(NOT EXISTS ${VCPKG_EXEC})
    message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
  endif()

  set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

  message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
  execute_process(COMMAND ${VCPKG_EXEC} install ${MY_PROJECT_DEPENDENCIES} WORKING_DIRECTORY ${VCPKG_ROOT})
endif()

project (vcpkg_test CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Boost 1.67 REQUIRED)

add_executable(driver src/driver.cpp)
target_include_directories(driver PUBLIC ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include )

enable_testing()
include(CTest)
add_executable(driver_test ${PROJECT_SOURCE_DIR}/test/driver_test.cpp)
add_test(NAME driver COMMAND driver_test)
