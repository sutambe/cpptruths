cmake_minimum_required (VERSION 3.12)
include("cmake/HunterGate.cmake")
HunterGate(URL "https://github.com/ruslo/hunter/archive/v0.23.165.tar.gz"
           SHA1 "5a73f91df5f6109c0bb1104d0c0ee423f7bece79")

project (hunter_test CXX)
set(CMAKE_CXX_STANDARD 17)
hunter_add_package(Boost COMPONENTS filesystem)
find_package(Boost 1.67 CONFIG REQUIRED COMPONENTS filesystem)

add_executable(driver src/driver.cpp)
target_include_directories(driver PUBLIC ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(driver PUBLIC Boost::filesystem)

enable_testing()
include(CTest)
add_executable(driver_test ${PROJECT_SOURCE_DIR}/test/driver_test.cpp)
add_test(NAME driver COMMAND driver_test)
