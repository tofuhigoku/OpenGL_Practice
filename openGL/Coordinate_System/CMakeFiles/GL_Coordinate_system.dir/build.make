# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ms/openGL/Coordinate_System

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ms/openGL/Coordinate_System

# Include any dependencies generated for this target.
include CMakeFiles/GL_Coordinate_system.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GL_Coordinate_system.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GL_Coordinate_system.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GL_Coordinate_system.dir/flags.make

CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o: CMakeFiles/GL_Coordinate_system.dir/flags.make
CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o: Coordinate_system.cpp
CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o: CMakeFiles/GL_Coordinate_system.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ms/openGL/Coordinate_System/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o -MF CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o.d -o CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o -c /home/ms/openGL/Coordinate_System/Coordinate_system.cpp

CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ms/openGL/Coordinate_System/Coordinate_system.cpp > CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.i

CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ms/openGL/Coordinate_System/Coordinate_system.cpp -o CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.s

# Object files for target GL_Coordinate_system
GL_Coordinate_system_OBJECTS = \
"CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o"

# External object files for target GL_Coordinate_system
GL_Coordinate_system_EXTERNAL_OBJECTS =

GL_Coordinate_system: CMakeFiles/GL_Coordinate_system.dir/Coordinate_system.cpp.o
GL_Coordinate_system: CMakeFiles/GL_Coordinate_system.dir/build.make
GL_Coordinate_system: CMakeFiles/GL_Coordinate_system.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ms/openGL/Coordinate_System/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GL_Coordinate_system"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GL_Coordinate_system.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GL_Coordinate_system.dir/build: GL_Coordinate_system
.PHONY : CMakeFiles/GL_Coordinate_system.dir/build

CMakeFiles/GL_Coordinate_system.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GL_Coordinate_system.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GL_Coordinate_system.dir/clean

CMakeFiles/GL_Coordinate_system.dir/depend:
	cd /home/ms/openGL/Coordinate_System && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ms/openGL/Coordinate_System /home/ms/openGL/Coordinate_System /home/ms/openGL/Coordinate_System /home/ms/openGL/Coordinate_System /home/ms/openGL/Coordinate_System/CMakeFiles/GL_Coordinate_system.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GL_Coordinate_system.dir/depend

