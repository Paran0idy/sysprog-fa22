# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/twoballs/courses/sysprog-fa22/lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug

# Utility rule file for ZydisDoc.

# Include the progress variables for this target.
include deps/zydis/CMakeFiles/ZydisDoc.dir/progress.make

deps/zydis/CMakeFiles/ZydisDoc: deps/zydis/Doxyfile.ZydisDoc
deps/zydis/CMakeFiles/ZydisDoc: ../deps/zydis/README.md
deps/zydis/CMakeFiles/ZydisDoc: ../deps/zydis/files.dox
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generate API documentation for ZydisDoc"
	cd /home/twoballs/courses/sysprog-fa22/lab3/deps/zydis && /usr/bin/cmake -E make_directory /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/deps/zydis
	cd /home/twoballs/courses/sysprog-fa22/lab3/deps/zydis && /usr/bin/doxygen /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/deps/zydis/Doxyfile.ZydisDoc

ZydisDoc: deps/zydis/CMakeFiles/ZydisDoc
ZydisDoc: deps/zydis/CMakeFiles/ZydisDoc.dir/build.make

.PHONY : ZydisDoc

# Rule to build all files generated by this target.
deps/zydis/CMakeFiles/ZydisDoc.dir/build: ZydisDoc

.PHONY : deps/zydis/CMakeFiles/ZydisDoc.dir/build

deps/zydis/CMakeFiles/ZydisDoc.dir/clean:
	cd /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/deps/zydis && $(CMAKE_COMMAND) -P CMakeFiles/ZydisDoc.dir/cmake_clean.cmake
.PHONY : deps/zydis/CMakeFiles/ZydisDoc.dir/clean

deps/zydis/CMakeFiles/ZydisDoc.dir/depend:
	cd /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/twoballs/courses/sysprog-fa22/lab3 /home/twoballs/courses/sysprog-fa22/lab3/deps/zydis /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/deps/zydis /home/twoballs/courses/sysprog-fa22/lab3/cmake-build-debug/deps/zydis/CMakeFiles/ZydisDoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/zydis/CMakeFiles/ZydisDoc.dir/depend

