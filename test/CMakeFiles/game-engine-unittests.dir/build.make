# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.4.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.4.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/brian/Dropbox/projects/cpp/game-engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/brian/Dropbox/projects/cpp/game-engine

# Include any dependencies generated for this target.
include test/CMakeFiles/game-engine-unittests.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/game-engine-unittests.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/game-engine-unittests.dir/flags.make

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o: test/CMakeFiles/game-engine-unittests.dir/flags.make
test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o: test/Entity.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brian/Dropbox/projects/cpp/game-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/game-engine-unittests.dir/Entity.cpp.o -c /Users/brian/Dropbox/projects/cpp/game-engine/test/Entity.cpp

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/game-engine-unittests.dir/Entity.cpp.i"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brian/Dropbox/projects/cpp/game-engine/test/Entity.cpp > CMakeFiles/game-engine-unittests.dir/Entity.cpp.i

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/game-engine-unittests.dir/Entity.cpp.s"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brian/Dropbox/projects/cpp/game-engine/test/Entity.cpp -o CMakeFiles/game-engine-unittests.dir/Entity.cpp.s

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.requires:

.PHONY : test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.requires

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.provides: test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/game-engine-unittests.dir/build.make test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.provides.build
.PHONY : test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.provides

test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.provides.build: test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o


test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o: test/CMakeFiles/game-engine-unittests.dir/flags.make
test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o: test/FactoryTuple.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/brian/Dropbox/projects/cpp/game-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o -c /Users/brian/Dropbox/projects/cpp/game-engine/test/FactoryTuple.cpp

test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.i"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/brian/Dropbox/projects/cpp/game-engine/test/FactoryTuple.cpp > CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.i

test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.s"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && /usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/brian/Dropbox/projects/cpp/game-engine/test/FactoryTuple.cpp -o CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.s

test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.requires:

.PHONY : test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.requires

test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.provides: test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/game-engine-unittests.dir/build.make test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.provides.build
.PHONY : test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.provides

test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.provides.build: test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o


# Object files for target game-engine-unittests
game__engine__unittests_OBJECTS = \
"CMakeFiles/game-engine-unittests.dir/Entity.cpp.o" \
"CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o"

# External object files for target game-engine-unittests
game__engine__unittests_EXTERNAL_OBJECTS =

bin/game-engine-unittests: test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o
bin/game-engine-unittests: test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o
bin/game-engine-unittests: test/CMakeFiles/game-engine-unittests.dir/build.make
bin/game-engine-unittests: /usr/local/lib/libsfml-system.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-window.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-graphics.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-network.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-audio.dylib
bin/game-engine-unittests: lib/libprojectimpl.a
bin/game-engine-unittests: /usr/local/lib/libsfml-system.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-window.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-graphics.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-network.dylib
bin/game-engine-unittests: /usr/local/lib/libsfml-audio.dylib
bin/game-engine-unittests: lib/libgtest.a
bin/game-engine-unittests: lib/libgtest_main.a
bin/game-engine-unittests: lib/libgmock.a
bin/game-engine-unittests: lib/libgmock_main.a
bin/game-engine-unittests: lib/libgtest.a
bin/game-engine-unittests: test/CMakeFiles/game-engine-unittests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/brian/Dropbox/projects/cpp/game-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/game-engine-unittests"
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/game-engine-unittests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/game-engine-unittests.dir/build: bin/game-engine-unittests

.PHONY : test/CMakeFiles/game-engine-unittests.dir/build

test/CMakeFiles/game-engine-unittests.dir/requires: test/CMakeFiles/game-engine-unittests.dir/Entity.cpp.o.requires
test/CMakeFiles/game-engine-unittests.dir/requires: test/CMakeFiles/game-engine-unittests.dir/FactoryTuple.cpp.o.requires

.PHONY : test/CMakeFiles/game-engine-unittests.dir/requires

test/CMakeFiles/game-engine-unittests.dir/clean:
	cd /Users/brian/Dropbox/projects/cpp/game-engine/test && $(CMAKE_COMMAND) -P CMakeFiles/game-engine-unittests.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/game-engine-unittests.dir/clean

test/CMakeFiles/game-engine-unittests.dir/depend:
	cd /Users/brian/Dropbox/projects/cpp/game-engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/brian/Dropbox/projects/cpp/game-engine /Users/brian/Dropbox/projects/cpp/game-engine/test /Users/brian/Dropbox/projects/cpp/game-engine /Users/brian/Dropbox/projects/cpp/game-engine/test /Users/brian/Dropbox/projects/cpp/game-engine/test/CMakeFiles/game-engine-unittests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/game-engine-unittests.dir/depend

