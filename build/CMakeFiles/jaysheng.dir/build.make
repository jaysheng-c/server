# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /apps/yoshino/bin/cmake

# The command to remove a file.
RM = /apps/yoshino/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/server/build

# Include any dependencies generated for this target.
include CMakeFiles/jaysheng.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/jaysheng.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jaysheng.dir/flags.make

CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o: CMakeFiles/jaysheng.dir/flags.make
CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o: ../jaysheng/log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o -c /root/workspace/server/jaysheng/log.cpp

CMakeFiles/jaysheng.dir/jaysheng/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jaysheng.dir/jaysheng/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/server/jaysheng/log.cpp > CMakeFiles/jaysheng.dir/jaysheng/log.cpp.i

CMakeFiles/jaysheng.dir/jaysheng/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jaysheng.dir/jaysheng/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/server/jaysheng/log.cpp -o CMakeFiles/jaysheng.dir/jaysheng/log.cpp.s

CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o: CMakeFiles/jaysheng.dir/flags.make
CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o: ../jaysheng/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o -c /root/workspace/server/jaysheng/util.cpp

CMakeFiles/jaysheng.dir/jaysheng/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jaysheng.dir/jaysheng/util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/server/jaysheng/util.cpp > CMakeFiles/jaysheng.dir/jaysheng/util.cpp.i

CMakeFiles/jaysheng.dir/jaysheng/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jaysheng.dir/jaysheng/util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/server/jaysheng/util.cpp -o CMakeFiles/jaysheng.dir/jaysheng/util.cpp.s

CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o: CMakeFiles/jaysheng.dir/flags.make
CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o: ../jaysheng/config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o -c /root/workspace/server/jaysheng/config.cpp

CMakeFiles/jaysheng.dir/jaysheng/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jaysheng.dir/jaysheng/config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/server/jaysheng/config.cpp > CMakeFiles/jaysheng.dir/jaysheng/config.cpp.i

CMakeFiles/jaysheng.dir/jaysheng/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jaysheng.dir/jaysheng/config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/server/jaysheng/config.cpp -o CMakeFiles/jaysheng.dir/jaysheng/config.cpp.s

# Object files for target jaysheng
jaysheng_OBJECTS = \
"CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o" \
"CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o" \
"CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o"

# External object files for target jaysheng
jaysheng_EXTERNAL_OBJECTS =

../lib/libjaysheng.so: CMakeFiles/jaysheng.dir/jaysheng/log.cpp.o
../lib/libjaysheng.so: CMakeFiles/jaysheng.dir/jaysheng/util.cpp.o
../lib/libjaysheng.so: CMakeFiles/jaysheng.dir/jaysheng/config.cpp.o
../lib/libjaysheng.so: CMakeFiles/jaysheng.dir/build.make
../lib/libjaysheng.so: CMakeFiles/jaysheng.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../lib/libjaysheng.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jaysheng.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jaysheng.dir/build: ../lib/libjaysheng.so

.PHONY : CMakeFiles/jaysheng.dir/build

CMakeFiles/jaysheng.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jaysheng.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jaysheng.dir/clean

CMakeFiles/jaysheng.dir/depend:
	cd /root/workspace/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/server /root/workspace/server /root/workspace/server/build /root/workspace/server/build /root/workspace/server/build/CMakeFiles/jaysheng.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/jaysheng.dir/depend
