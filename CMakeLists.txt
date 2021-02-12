cmake_minimum_required(VERSION 3.0)
project(server)

set(PROJECT_ROOT "${PROJECT_SOURCE_DIR}")
set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_BINARY_DIR "${PROJECT_ROOT}/build")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_ROOT}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_ROOT}/lib")

set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -lstdc++ -O3 -ggdb -fPIC -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
# set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -lstdc -O3 -ggdb -fPIC -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -fPIC -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
# set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -fPIC -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
# set(CMAKE_CXX_FLAGS "-std=c++11")

message("CMAKE_HOME_DIRECTORY " ${CMAKE_HOME_DIRECTORY})

include_directories(.)
include_directories(/apps/yoshino/include)

link_directories(/apps/yoshino/lib)
link_directories(/apps/yoshino/lib64)

message("CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})
# link_directories(/usr/local/lib64)

find_package(yaml-cpp)

set(JAYSHENG_LIB jaysheng/log.cpp
                 jaysheng/util.cpp
                 jaysheng/config.cpp
                 jaysheng/mutex.cpp
                 jaysheng/thread.cpp
                 jaysheng/fiber.cpp
    )

add_library(jaysheng SHARED ${JAYSHENG_LIB})

set(LIBS
        ${jaysheng}
        ${YAMLCPP}
        # ${PTHREAD}
        # yaml-cpp
    )
# message ("LIBS " ${LIBS})

add_executable(test_log test/test_log.cpp )
add_dependencies(test_log jaysheng)
target_link_libraries(test_log jaysheng -lyaml-cpp -pthread)
# target_link_libraries(test_log ${LIBS})

add_executable(test_config test/test_config.cpp)
add_dependencies(test_config jaysheng)
target_link_libraries(test_config jaysheng -lyaml-cpp -pthread)

add_executable(test_yaml test/main.cpp)
target_link_libraries(test_yaml yaml-cpp)

add_executable(test_thread test/test_thread.cpp)
add_dependencies(test_thread jaysheng)
target_link_libraries(test_thread -pthread jaysheng -lyaml-cpp )

add_executable(test_fiber test/test_fiber.cpp)
add_dependencies(test_fiber jaysheng)
target_link_libraries(test_fiber -pthread jaysheng -lyaml-cpp )
