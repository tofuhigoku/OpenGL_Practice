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
CMAKE_SOURCE_DIR = /home/ms/OpenGL_Practice/openGL/Model_loading

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ms/OpenGL_Practice/openGL/Model_loading

# Include any dependencies generated for this target.
include CMakeFiles/GL_ModelLoading.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GL_ModelLoading.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GL_ModelLoading.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GL_ModelLoading.dir/flags.make

CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o: CMakeFiles/GL_ModelLoading.dir/flags.make
CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o: Model_loading.cpp
CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o: CMakeFiles/GL_ModelLoading.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ms/OpenGL_Practice/openGL/Model_loading/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o -MF CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o.d -o CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o -c /home/ms/OpenGL_Practice/openGL/Model_loading/Model_loading.cpp

CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ms/OpenGL_Practice/openGL/Model_loading/Model_loading.cpp > CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.i

CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ms/OpenGL_Practice/openGL/Model_loading/Model_loading.cpp -o CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.s

# Object files for target GL_ModelLoading
GL_ModelLoading_OBJECTS = \
"CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o"

# External object files for target GL_ModelLoading
GL_ModelLoading_EXTERNAL_OBJECTS =

GL_ModelLoading: CMakeFiles/GL_ModelLoading.dir/Model_loading.cpp.o
GL_ModelLoading: CMakeFiles/GL_ModelLoading.dir/build.make
GL_ModelLoading: CMakeFiles/GL_ModelLoading.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ms/OpenGL_Practice/openGL/Model_loading/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GL_ModelLoading"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GL_ModelLoading.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GL_ModelLoading.dir/build: GL_ModelLoading
.PHONY : CMakeFiles/GL_ModelLoading.dir/build

CMakeFiles/GL_ModelLoading.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GL_ModelLoading.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GL_ModelLoading.dir/clean

CMakeFiles/GL_ModelLoading.dir/depend:
	cd /home/ms/OpenGL_Practice/openGL/Model_loading && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ms/OpenGL_Practice/openGL/Model_loading /home/ms/OpenGL_Practice/openGL/Model_loading /home/ms/OpenGL_Practice/openGL/Model_loading /home/ms/OpenGL_Practice/openGL/Model_loading /home/ms/OpenGL_Practice/openGL/Model_loading/CMakeFiles/GL_ModelLoading.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GL_ModelLoading.dir/depend

