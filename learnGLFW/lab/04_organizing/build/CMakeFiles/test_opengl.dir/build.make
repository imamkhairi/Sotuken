# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_SOURCE_DIR = /home/p214071/Sotuken/learnGLFW/lab/04_organizing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/p214071/Sotuken/learnGLFW/lab/04_organizing/build

# Include any dependencies generated for this target.
include CMakeFiles/test_opengl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_opengl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_opengl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_opengl.dir/flags.make

CMakeFiles/test_opengl.dir/src/04organizing.cpp.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/04organizing.cpp.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/04organizing.cpp
CMakeFiles/test_opengl.dir/src/04organizing.cpp.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_opengl.dir/src/04organizing.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/04organizing.cpp.o -MF CMakeFiles/test_opengl.dir/src/04organizing.cpp.o.d -o CMakeFiles/test_opengl.dir/src/04organizing.cpp.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/04organizing.cpp

CMakeFiles/test_opengl.dir/src/04organizing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_opengl.dir/src/04organizing.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/04organizing.cpp > CMakeFiles/test_opengl.dir/src/04organizing.cpp.i

CMakeFiles/test_opengl.dir/src/04organizing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_opengl.dir/src/04organizing.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/04organizing.cpp -o CMakeFiles/test_opengl.dir/src/04organizing.cpp.s

CMakeFiles/test_opengl.dir/src/glad.c.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/glad.c.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/glad.c
CMakeFiles/test_opengl.dir/src/glad.c.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test_opengl.dir/src/glad.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/glad.c.o -MF CMakeFiles/test_opengl.dir/src/glad.c.o.d -o CMakeFiles/test_opengl.dir/src/glad.c.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/glad.c

CMakeFiles/test_opengl.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_opengl.dir/src/glad.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/glad.c > CMakeFiles/test_opengl.dir/src/glad.c.i

CMakeFiles/test_opengl.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_opengl.dir/src/glad.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/glad.c -o CMakeFiles/test_opengl.dir/src/glad.c.s

CMakeFiles/test_opengl.dir/src/EBO.cpp.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/EBO.cpp.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/EBO.cpp
CMakeFiles/test_opengl.dir/src/EBO.cpp.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_opengl.dir/src/EBO.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/EBO.cpp.o -MF CMakeFiles/test_opengl.dir/src/EBO.cpp.o.d -o CMakeFiles/test_opengl.dir/src/EBO.cpp.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/EBO.cpp

CMakeFiles/test_opengl.dir/src/EBO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_opengl.dir/src/EBO.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/EBO.cpp > CMakeFiles/test_opengl.dir/src/EBO.cpp.i

CMakeFiles/test_opengl.dir/src/EBO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_opengl.dir/src/EBO.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/EBO.cpp -o CMakeFiles/test_opengl.dir/src/EBO.cpp.s

CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/shaderClass.cpp
CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o -MF CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o.d -o CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/shaderClass.cpp

CMakeFiles/test_opengl.dir/src/shaderClass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_opengl.dir/src/shaderClass.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/shaderClass.cpp > CMakeFiles/test_opengl.dir/src/shaderClass.cpp.i

CMakeFiles/test_opengl.dir/src/shaderClass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_opengl.dir/src/shaderClass.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/shaderClass.cpp -o CMakeFiles/test_opengl.dir/src/shaderClass.cpp.s

CMakeFiles/test_opengl.dir/src/VAO.cpp.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/VAO.cpp.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VAO.cpp
CMakeFiles/test_opengl.dir/src/VAO.cpp.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_opengl.dir/src/VAO.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/VAO.cpp.o -MF CMakeFiles/test_opengl.dir/src/VAO.cpp.o.d -o CMakeFiles/test_opengl.dir/src/VAO.cpp.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VAO.cpp

CMakeFiles/test_opengl.dir/src/VAO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_opengl.dir/src/VAO.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VAO.cpp > CMakeFiles/test_opengl.dir/src/VAO.cpp.i

CMakeFiles/test_opengl.dir/src/VAO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_opengl.dir/src/VAO.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VAO.cpp -o CMakeFiles/test_opengl.dir/src/VAO.cpp.s

CMakeFiles/test_opengl.dir/src/VBO.cpp.o: CMakeFiles/test_opengl.dir/flags.make
CMakeFiles/test_opengl.dir/src/VBO.cpp.o: /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VBO.cpp
CMakeFiles/test_opengl.dir/src/VBO.cpp.o: CMakeFiles/test_opengl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_opengl.dir/src/VBO.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_opengl.dir/src/VBO.cpp.o -MF CMakeFiles/test_opengl.dir/src/VBO.cpp.o.d -o CMakeFiles/test_opengl.dir/src/VBO.cpp.o -c /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VBO.cpp

CMakeFiles/test_opengl.dir/src/VBO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_opengl.dir/src/VBO.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VBO.cpp > CMakeFiles/test_opengl.dir/src/VBO.cpp.i

CMakeFiles/test_opengl.dir/src/VBO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_opengl.dir/src/VBO.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/p214071/Sotuken/learnGLFW/lab/04_organizing/src/VBO.cpp -o CMakeFiles/test_opengl.dir/src/VBO.cpp.s

# Object files for target test_opengl
test_opengl_OBJECTS = \
"CMakeFiles/test_opengl.dir/src/04organizing.cpp.o" \
"CMakeFiles/test_opengl.dir/src/glad.c.o" \
"CMakeFiles/test_opengl.dir/src/EBO.cpp.o" \
"CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o" \
"CMakeFiles/test_opengl.dir/src/VAO.cpp.o" \
"CMakeFiles/test_opengl.dir/src/VBO.cpp.o"

# External object files for target test_opengl
test_opengl_EXTERNAL_OBJECTS =

test_opengl: CMakeFiles/test_opengl.dir/src/04organizing.cpp.o
test_opengl: CMakeFiles/test_opengl.dir/src/glad.c.o
test_opengl: CMakeFiles/test_opengl.dir/src/EBO.cpp.o
test_opengl: CMakeFiles/test_opengl.dir/src/shaderClass.cpp.o
test_opengl: CMakeFiles/test_opengl.dir/src/VAO.cpp.o
test_opengl: CMakeFiles/test_opengl.dir/src/VBO.cpp.o
test_opengl: CMakeFiles/test_opengl.dir/build.make
test_opengl: CMakeFiles/test_opengl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable test_opengl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_opengl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_opengl.dir/build: test_opengl
.PHONY : CMakeFiles/test_opengl.dir/build

CMakeFiles/test_opengl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_opengl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_opengl.dir/clean

CMakeFiles/test_opengl.dir/depend:
	cd /home/p214071/Sotuken/learnGLFW/lab/04_organizing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/p214071/Sotuken/learnGLFW/lab/04_organizing /home/p214071/Sotuken/learnGLFW/lab/04_organizing /home/p214071/Sotuken/learnGLFW/lab/04_organizing/build /home/p214071/Sotuken/learnGLFW/lab/04_organizing/build /home/p214071/Sotuken/learnGLFW/lab/04_organizing/build/CMakeFiles/test_opengl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_opengl.dir/depend
