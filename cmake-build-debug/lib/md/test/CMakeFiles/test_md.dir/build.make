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
CMAKE_COMMAND = /snap/clion/103/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/103/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jhannah/zeus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jhannah/zeus/cmake-build-debug

# Include any dependencies generated for this target.
include lib/md/test/CMakeFiles/test_md.dir/depend.make

# Include the progress variables for this target.
include lib/md/test/CMakeFiles/test_md.dir/progress.make

# Include the compile flags for this target's objects.
include lib/md/test/CMakeFiles/test_md.dir/flags.make

lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.o: lib/md/test/CMakeFiles/test_md.dir/flags.make
lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.o: ../lib/md/test/test_book.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jhannah/zeus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.o"
	cd /home/jhannah/zeus/cmake-build-debug/lib/md/test && /usr/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_md.dir/test_book.cpp.o -c /home/jhannah/zeus/lib/md/test/test_book.cpp

lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_md.dir/test_book.cpp.i"
	cd /home/jhannah/zeus/cmake-build-debug/lib/md/test && /usr/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jhannah/zeus/lib/md/test/test_book.cpp > CMakeFiles/test_md.dir/test_book.cpp.i

lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_md.dir/test_book.cpp.s"
	cd /home/jhannah/zeus/cmake-build-debug/lib/md/test && /usr/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jhannah/zeus/lib/md/test/test_book.cpp -o CMakeFiles/test_md.dir/test_book.cpp.s

# Object files for target test_md
test_md_OBJECTS = \
"CMakeFiles/test_md.dir/test_book.cpp.o"

# External object files for target test_md
test_md_EXTERNAL_OBJECTS =

build/test/test_md: lib/md/test/CMakeFiles/test_md.dir/test_book.cpp.o
build/test/test_md: lib/md/test/CMakeFiles/test_md.dir/build.make
build/test/test_md: build/lib/libzeus_md.a
build/test/test_md: build/lib/libzeus_core.a
build/test/test_md: build/lib/libzeus_math.a
build/test/test_md: build/lib/libzeus_system.a
build/test/test_md: build/lib/libzeus_time.a
build/test/test_md: lib/md/test/CMakeFiles/test_md.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jhannah/zeus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../build/test/test_md"
	cd /home/jhannah/zeus/cmake-build-debug/lib/md/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_md.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/md/test/CMakeFiles/test_md.dir/build: build/test/test_md

.PHONY : lib/md/test/CMakeFiles/test_md.dir/build

lib/md/test/CMakeFiles/test_md.dir/clean:
	cd /home/jhannah/zeus/cmake-build-debug/lib/md/test && $(CMAKE_COMMAND) -P CMakeFiles/test_md.dir/cmake_clean.cmake
.PHONY : lib/md/test/CMakeFiles/test_md.dir/clean

lib/md/test/CMakeFiles/test_md.dir/depend:
	cd /home/jhannah/zeus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jhannah/zeus /home/jhannah/zeus/lib/md/test /home/jhannah/zeus/cmake-build-debug /home/jhannah/zeus/cmake-build-debug/lib/md/test /home/jhannah/zeus/cmake-build-debug/lib/md/test/CMakeFiles/test_md.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/md/test/CMakeFiles/test_md.dir/depend
