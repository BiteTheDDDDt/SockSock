# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /root/program/CPP/SockSock

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/program/CPP/SockSock

# Include any dependencies generated for this target.
include CMakeFiles/SockServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SockServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SockServer.dir/flags.make

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o: CMakeFiles/SockServer.dir/flags.make
CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o: Sources/ServerMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/program/CPP/SockSock/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o"
	/usr/bin/g++-5   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o -c /root/program/CPP/SockSock/Sources/ServerMain.cpp

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.i"
	/usr/bin/g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/program/CPP/SockSock/Sources/ServerMain.cpp > CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.i

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.s"
	/usr/bin/g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/program/CPP/SockSock/Sources/ServerMain.cpp -o CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.s

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.requires:

.PHONY : CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.requires

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.provides: CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.requires
	$(MAKE) -f CMakeFiles/SockServer.dir/build.make CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.provides.build
.PHONY : CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.provides

CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.provides.build: CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o


CMakeFiles/SockServer.dir/Sources/Server.cpp.o: CMakeFiles/SockServer.dir/flags.make
CMakeFiles/SockServer.dir/Sources/Server.cpp.o: Sources/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/program/CPP/SockSock/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SockServer.dir/Sources/Server.cpp.o"
	/usr/bin/g++-5   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SockServer.dir/Sources/Server.cpp.o -c /root/program/CPP/SockSock/Sources/Server.cpp

CMakeFiles/SockServer.dir/Sources/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SockServer.dir/Sources/Server.cpp.i"
	/usr/bin/g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/program/CPP/SockSock/Sources/Server.cpp > CMakeFiles/SockServer.dir/Sources/Server.cpp.i

CMakeFiles/SockServer.dir/Sources/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SockServer.dir/Sources/Server.cpp.s"
	/usr/bin/g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/program/CPP/SockSock/Sources/Server.cpp -o CMakeFiles/SockServer.dir/Sources/Server.cpp.s

CMakeFiles/SockServer.dir/Sources/Server.cpp.o.requires:

.PHONY : CMakeFiles/SockServer.dir/Sources/Server.cpp.o.requires

CMakeFiles/SockServer.dir/Sources/Server.cpp.o.provides: CMakeFiles/SockServer.dir/Sources/Server.cpp.o.requires
	$(MAKE) -f CMakeFiles/SockServer.dir/build.make CMakeFiles/SockServer.dir/Sources/Server.cpp.o.provides.build
.PHONY : CMakeFiles/SockServer.dir/Sources/Server.cpp.o.provides

CMakeFiles/SockServer.dir/Sources/Server.cpp.o.provides.build: CMakeFiles/SockServer.dir/Sources/Server.cpp.o


# Object files for target SockServer
SockServer_OBJECTS = \
"CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o" \
"CMakeFiles/SockServer.dir/Sources/Server.cpp.o"

# External object files for target SockServer
SockServer_EXTERNAL_OBJECTS =

SockServer: CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o
SockServer: CMakeFiles/SockServer.dir/Sources/Server.cpp.o
SockServer: CMakeFiles/SockServer.dir/build.make
SockServer: CMakeFiles/SockServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/program/CPP/SockSock/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable SockServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SockServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SockServer.dir/build: SockServer

.PHONY : CMakeFiles/SockServer.dir/build

CMakeFiles/SockServer.dir/requires: CMakeFiles/SockServer.dir/Sources/ServerMain.cpp.o.requires
CMakeFiles/SockServer.dir/requires: CMakeFiles/SockServer.dir/Sources/Server.cpp.o.requires

.PHONY : CMakeFiles/SockServer.dir/requires

CMakeFiles/SockServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SockServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SockServer.dir/clean

CMakeFiles/SockServer.dir/depend:
	cd /root/program/CPP/SockSock && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/program/CPP/SockSock /root/program/CPP/SockSock /root/program/CPP/SockSock /root/program/CPP/SockSock /root/program/CPP/SockSock/CMakeFiles/SockServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SockServer.dir/depend
