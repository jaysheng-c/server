cmake_minimum_required(VERSION 3.0)
project(server)

set(PROJECT_ROOT "${PROJECT_SOURCE_DIR}")
set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(PROJECT_BINARY_DIR "${PROJECT_ROOT}/build")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_ROOT}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_ROOT}/lib")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -ggdb -fPIC -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -O3 -ggdb -fPIC -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")


include_directories(/apps/yoshino/include)
link_directories(/apps/yoshino/lib)
link_directories(/apps/yoshino/lib64)


#set(LIBS_SRC xxx)


#add_library(LIBS STATIC ${LIBS_SRC})


add_executable(server test/main.cpp)
#target_link_libraries(project ${LIBS})
