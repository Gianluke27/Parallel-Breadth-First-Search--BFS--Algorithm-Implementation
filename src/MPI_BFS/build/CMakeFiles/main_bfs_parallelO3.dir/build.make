# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build

# Include any dependencies generated for this target.
include CMakeFiles/main_bfs_parallelO3.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main_bfs_parallelO3.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main_bfs_parallelO3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_bfs_parallelO3.dir/flags.make

CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o: CMakeFiles/main_bfs_parallelO3.dir/flags.make
CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o: ../main_bfs_parallel.c
CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o: CMakeFiles/main_bfs_parallelO3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o -MF CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o.d -o CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o -c /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/main_bfs_parallel.c

CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/main_bfs_parallel.c > CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.i

CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/main_bfs_parallel.c -o CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.s

# Object files for target main_bfs_parallelO3
main_bfs_parallelO3_OBJECTS = \
"CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o"

# External object files for target main_bfs_parallelO3
main_bfs_parallelO3_EXTERNAL_OBJECTS =

main_bfs_parallelO3: CMakeFiles/main_bfs_parallelO3.dir/main_bfs_parallel.c.o
main_bfs_parallelO3: CMakeFiles/main_bfs_parallelO3.dir/build.make
main_bfs_parallelO3: /usr/lib/x86_64-linux-gnu/libmpich.so
main_bfs_parallelO3: CMakeFiles/main_bfs_parallelO3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable main_bfs_parallelO3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_bfs_parallelO3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_bfs_parallelO3.dir/build: main_bfs_parallelO3
.PHONY : CMakeFiles/main_bfs_parallelO3.dir/build

CMakeFiles/main_bfs_parallelO3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_bfs_parallelO3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_bfs_parallelO3.dir/clean

CMakeFiles/main_bfs_parallelO3.dir/depend:
	cd /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build /mnt/c/Users/Gianluca/Desktop/Secondo_appello/ContestMPI_bfs/build/CMakeFiles/main_bfs_parallelO3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_bfs_parallelO3.dir/depend

