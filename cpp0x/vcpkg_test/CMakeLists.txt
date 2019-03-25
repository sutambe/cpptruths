cmake_minimum_required (VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(AutoVcpkg)
vcpkg_install(boost-core boost-optional)
message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

project (vcpkg_test CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Boost 1.67 REQUIRED)

add_executable(driver src/driver.cpp)
target_include_directories(driver PUBLIC ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include )

enable_testing()
include(CTest)
add_executable(driver_test ${PROJECT_SOURCE_DIR}/test/driver_test.cpp)
add_test(NAME driver COMMAND driver_test)
