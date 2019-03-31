cmake_minimum_required (VERSION 3.12)

if(NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
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

	if(WIN32)
		if(NOT EXISTS ${VCPKG_ROOT}/vcpkg.exe)
		  message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
		  execute_process(COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.bat WORKING_DIRECTORY ${VCPKG_ROOT})
		endif()

		if(NOT EXISTS ${VCPKG_ROOT}/vcpkg.exe)
		  message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
		endif()

		set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

		set(MY_PROJECT_DEPENDENCIES boost-core boost-optional)
		message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
		execute_process(COMMAND ${VCPKG_ROOT}/vcpkg.exe install ${MY_PROJECT_DEPENDENCIES} WORKING_DIRECTORY ${VCPKG_ROOT})
	endif()
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
