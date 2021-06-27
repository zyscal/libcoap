# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = "/home/zyscal/Documents/CoAP over TCP/libcoap"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/zyscal/Documents/CoAP over TCP/libcoap"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"dev\" \"lib\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/home/zyscal/Documents/CoAP over TCP/libcoap/CMakeFiles" "/home/zyscal/Documents/CoAP over TCP/libcoap/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/home/zyscal/Documents/CoAP over TCP/libcoap/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named coap-3

# Build rule for target.
coap-3: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 coap-3
.PHONY : coap-3

# fast build rule for target.
coap-3/fast:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/build
.PHONY : coap-3/fast

#=============================================================================
# Target rules for targets named coap-client

# Build rule for target.
coap-client: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 coap-client
.PHONY : coap-client

# fast build rule for target.
coap-client/fast:
	$(MAKE) -f CMakeFiles/coap-client.dir/build.make CMakeFiles/coap-client.dir/build
.PHONY : coap-client/fast

#=============================================================================
# Target rules for targets named coap-rd

# Build rule for target.
coap-rd: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 coap-rd
.PHONY : coap-rd

# fast build rule for target.
coap-rd/fast:
	$(MAKE) -f CMakeFiles/coap-rd.dir/build.make CMakeFiles/coap-rd.dir/build
.PHONY : coap-rd/fast

#=============================================================================
# Target rules for targets named QoS_organizer

# Build rule for target.
QoS_organizer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 QoS_organizer
.PHONY : QoS_organizer

# fast build rule for target.
QoS_organizer/fast:
	$(MAKE) -f CMakeFiles/QoS_organizer.dir/build.make CMakeFiles/QoS_organizer.dir/build
.PHONY : QoS_organizer/fast

#=============================================================================
# Target rules for targets named coap-server

# Build rule for target.
coap-server: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 coap-server
.PHONY : coap-server

# fast build rule for target.
coap-server/fast:
	$(MAKE) -f CMakeFiles/coap-server.dir/build.make CMakeFiles/coap-server.dir/build
.PHONY : coap-server/fast

#=============================================================================
# Target rules for targets named QoS_analyzer

# Build rule for target.
QoS_analyzer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 QoS_analyzer
.PHONY : QoS_analyzer

# fast build rule for target.
QoS_analyzer/fast:
	$(MAKE) -f CMakeFiles/QoS_analyzer.dir/build.make CMakeFiles/QoS_analyzer.dir/build
.PHONY : QoS_analyzer/fast

#=============================================================================
# Target rules for targets named tiny

# Build rule for target.
tiny: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 tiny
.PHONY : tiny

# fast build rule for target.
tiny/fast:
	$(MAKE) -f CMakeFiles/tiny.dir/build.make CMakeFiles/tiny.dir/build
.PHONY : tiny/fast

#=============================================================================
# Target rules for targets named etsi_iot_01

# Build rule for target.
etsi_iot_01: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 etsi_iot_01
.PHONY : etsi_iot_01

# fast build rule for target.
etsi_iot_01/fast:
	$(MAKE) -f CMakeFiles/etsi_iot_01.dir/build.make CMakeFiles/etsi_iot_01.dir/build
.PHONY : etsi_iot_01/fast

#=============================================================================
# Target rules for targets named doc_doxygen

# Build rule for target.
doc_doxygen: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 doc_doxygen
.PHONY : doc_doxygen

# fast build rule for target.
doc_doxygen/fast:
	$(MAKE) -f CMakeFiles/doc_doxygen.dir/build.make CMakeFiles/doc_doxygen.dir/build
.PHONY : doc_doxygen/fast

examples/coap-client.o: examples/coap-client.c.o

.PHONY : examples/coap-client.o

# target to build an object file
examples/coap-client.c.o:
	$(MAKE) -f CMakeFiles/coap-client.dir/build.make CMakeFiles/coap-client.dir/examples/coap-client.c.o
.PHONY : examples/coap-client.c.o

examples/coap-client.i: examples/coap-client.c.i

.PHONY : examples/coap-client.i

# target to preprocess a source file
examples/coap-client.c.i:
	$(MAKE) -f CMakeFiles/coap-client.dir/build.make CMakeFiles/coap-client.dir/examples/coap-client.c.i
.PHONY : examples/coap-client.c.i

examples/coap-client.s: examples/coap-client.c.s

.PHONY : examples/coap-client.s

# target to generate assembly for a file
examples/coap-client.c.s:
	$(MAKE) -f CMakeFiles/coap-client.dir/build.make CMakeFiles/coap-client.dir/examples/coap-client.c.s
.PHONY : examples/coap-client.c.s

examples/coap-rd.o: examples/coap-rd.c.o

.PHONY : examples/coap-rd.o

# target to build an object file
examples/coap-rd.c.o:
	$(MAKE) -f CMakeFiles/coap-rd.dir/build.make CMakeFiles/coap-rd.dir/examples/coap-rd.c.o
.PHONY : examples/coap-rd.c.o

examples/coap-rd.i: examples/coap-rd.c.i

.PHONY : examples/coap-rd.i

# target to preprocess a source file
examples/coap-rd.c.i:
	$(MAKE) -f CMakeFiles/coap-rd.dir/build.make CMakeFiles/coap-rd.dir/examples/coap-rd.c.i
.PHONY : examples/coap-rd.c.i

examples/coap-rd.s: examples/coap-rd.c.s

.PHONY : examples/coap-rd.s

# target to generate assembly for a file
examples/coap-rd.c.s:
	$(MAKE) -f CMakeFiles/coap-rd.dir/build.make CMakeFiles/coap-rd.dir/examples/coap-rd.c.s
.PHONY : examples/coap-rd.c.s

examples/coap-server.o: examples/coap-server.c.o

.PHONY : examples/coap-server.o

# target to build an object file
examples/coap-server.c.o:
	$(MAKE) -f CMakeFiles/coap-server.dir/build.make CMakeFiles/coap-server.dir/examples/coap-server.c.o
.PHONY : examples/coap-server.c.o

examples/coap-server.i: examples/coap-server.c.i

.PHONY : examples/coap-server.i

# target to preprocess a source file
examples/coap-server.c.i:
	$(MAKE) -f CMakeFiles/coap-server.dir/build.make CMakeFiles/coap-server.dir/examples/coap-server.c.i
.PHONY : examples/coap-server.c.i

examples/coap-server.s: examples/coap-server.c.s

.PHONY : examples/coap-server.s

# target to generate assembly for a file
examples/coap-server.c.s:
	$(MAKE) -f CMakeFiles/coap-server.dir/build.make CMakeFiles/coap-server.dir/examples/coap-server.c.s
.PHONY : examples/coap-server.c.s

examples/etsi_iot_01.o: examples/etsi_iot_01.c.o

.PHONY : examples/etsi_iot_01.o

# target to build an object file
examples/etsi_iot_01.c.o:
	$(MAKE) -f CMakeFiles/etsi_iot_01.dir/build.make CMakeFiles/etsi_iot_01.dir/examples/etsi_iot_01.c.o
.PHONY : examples/etsi_iot_01.c.o

examples/etsi_iot_01.i: examples/etsi_iot_01.c.i

.PHONY : examples/etsi_iot_01.i

# target to preprocess a source file
examples/etsi_iot_01.c.i:
	$(MAKE) -f CMakeFiles/etsi_iot_01.dir/build.make CMakeFiles/etsi_iot_01.dir/examples/etsi_iot_01.c.i
.PHONY : examples/etsi_iot_01.c.i

examples/etsi_iot_01.s: examples/etsi_iot_01.c.s

.PHONY : examples/etsi_iot_01.s

# target to generate assembly for a file
examples/etsi_iot_01.c.s:
	$(MAKE) -f CMakeFiles/etsi_iot_01.dir/build.make CMakeFiles/etsi_iot_01.dir/examples/etsi_iot_01.c.s
.PHONY : examples/etsi_iot_01.c.s

examples/tiny.o: examples/tiny.c.o

.PHONY : examples/tiny.o

# target to build an object file
examples/tiny.c.o:
	$(MAKE) -f CMakeFiles/tiny.dir/build.make CMakeFiles/tiny.dir/examples/tiny.c.o
.PHONY : examples/tiny.c.o

examples/tiny.i: examples/tiny.c.i

.PHONY : examples/tiny.i

# target to preprocess a source file
examples/tiny.c.i:
	$(MAKE) -f CMakeFiles/tiny.dir/build.make CMakeFiles/tiny.dir/examples/tiny.c.i
.PHONY : examples/tiny.c.i

examples/tiny.s: examples/tiny.c.s

.PHONY : examples/tiny.s

# target to generate assembly for a file
examples/tiny.c.s:
	$(MAKE) -f CMakeFiles/tiny.dir/build.make CMakeFiles/tiny.dir/examples/tiny.c.s
.PHONY : examples/tiny.c.s

my_libcoap/QoS_analyzer.o: my_libcoap/QoS_analyzer.c.o

.PHONY : my_libcoap/QoS_analyzer.o

# target to build an object file
my_libcoap/QoS_analyzer.c.o:
	$(MAKE) -f CMakeFiles/QoS_analyzer.dir/build.make CMakeFiles/QoS_analyzer.dir/my_libcoap/QoS_analyzer.c.o
.PHONY : my_libcoap/QoS_analyzer.c.o

my_libcoap/QoS_analyzer.i: my_libcoap/QoS_analyzer.c.i

.PHONY : my_libcoap/QoS_analyzer.i

# target to preprocess a source file
my_libcoap/QoS_analyzer.c.i:
	$(MAKE) -f CMakeFiles/QoS_analyzer.dir/build.make CMakeFiles/QoS_analyzer.dir/my_libcoap/QoS_analyzer.c.i
.PHONY : my_libcoap/QoS_analyzer.c.i

my_libcoap/QoS_analyzer.s: my_libcoap/QoS_analyzer.c.s

.PHONY : my_libcoap/QoS_analyzer.s

# target to generate assembly for a file
my_libcoap/QoS_analyzer.c.s:
	$(MAKE) -f CMakeFiles/QoS_analyzer.dir/build.make CMakeFiles/QoS_analyzer.dir/my_libcoap/QoS_analyzer.c.s
.PHONY : my_libcoap/QoS_analyzer.c.s

my_libcoap/QoS_organizer.o: my_libcoap/QoS_organizer.c.o

.PHONY : my_libcoap/QoS_organizer.o

# target to build an object file
my_libcoap/QoS_organizer.c.o:
	$(MAKE) -f CMakeFiles/QoS_organizer.dir/build.make CMakeFiles/QoS_organizer.dir/my_libcoap/QoS_organizer.c.o
.PHONY : my_libcoap/QoS_organizer.c.o

my_libcoap/QoS_organizer.i: my_libcoap/QoS_organizer.c.i

.PHONY : my_libcoap/QoS_organizer.i

# target to preprocess a source file
my_libcoap/QoS_organizer.c.i:
	$(MAKE) -f CMakeFiles/QoS_organizer.dir/build.make CMakeFiles/QoS_organizer.dir/my_libcoap/QoS_organizer.c.i
.PHONY : my_libcoap/QoS_organizer.c.i

my_libcoap/QoS_organizer.s: my_libcoap/QoS_organizer.c.s

.PHONY : my_libcoap/QoS_organizer.s

# target to generate assembly for a file
my_libcoap/QoS_organizer.c.s:
	$(MAKE) -f CMakeFiles/QoS_organizer.dir/build.make CMakeFiles/QoS_organizer.dir/my_libcoap/QoS_organizer.c.s
.PHONY : my_libcoap/QoS_organizer.c.s

src/address.o: src/address.c.o

.PHONY : src/address.o

# target to build an object file
src/address.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/address.c.o
.PHONY : src/address.c.o

src/address.i: src/address.c.i

.PHONY : src/address.i

# target to preprocess a source file
src/address.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/address.c.i
.PHONY : src/address.c.i

src/address.s: src/address.c.s

.PHONY : src/address.s

# target to generate assembly for a file
src/address.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/address.c.s
.PHONY : src/address.c.s

src/async.o: src/async.c.o

.PHONY : src/async.o

# target to build an object file
src/async.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/async.c.o
.PHONY : src/async.c.o

src/async.i: src/async.c.i

.PHONY : src/async.i

# target to preprocess a source file
src/async.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/async.c.i
.PHONY : src/async.c.i

src/async.s: src/async.c.s

.PHONY : src/async.s

# target to generate assembly for a file
src/async.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/async.c.s
.PHONY : src/async.c.s

src/block.o: src/block.c.o

.PHONY : src/block.o

# target to build an object file
src/block.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/block.c.o
.PHONY : src/block.c.o

src/block.i: src/block.c.i

.PHONY : src/block.i

# target to preprocess a source file
src/block.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/block.c.i
.PHONY : src/block.c.i

src/block.s: src/block.c.s

.PHONY : src/block.s

# target to generate assembly for a file
src/block.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/block.c.s
.PHONY : src/block.c.s

src/coap_asn1.o: src/coap_asn1.c.o

.PHONY : src/coap_asn1.o

# target to build an object file
src/coap_asn1.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_asn1.c.o
.PHONY : src/coap_asn1.c.o

src/coap_asn1.i: src/coap_asn1.c.i

.PHONY : src/coap_asn1.i

# target to preprocess a source file
src/coap_asn1.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_asn1.c.i
.PHONY : src/coap_asn1.c.i

src/coap_asn1.s: src/coap_asn1.c.s

.PHONY : src/coap_asn1.s

# target to generate assembly for a file
src/coap_asn1.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_asn1.c.s
.PHONY : src/coap_asn1.c.s

src/coap_cache.o: src/coap_cache.c.o

.PHONY : src/coap_cache.o

# target to build an object file
src/coap_cache.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_cache.c.o
.PHONY : src/coap_cache.c.o

src/coap_cache.i: src/coap_cache.c.i

.PHONY : src/coap_cache.i

# target to preprocess a source file
src/coap_cache.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_cache.c.i
.PHONY : src/coap_cache.c.i

src/coap_cache.s: src/coap_cache.c.s

.PHONY : src/coap_cache.s

# target to generate assembly for a file
src/coap_cache.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_cache.c.s
.PHONY : src/coap_cache.c.s

src/coap_debug.o: src/coap_debug.c.o

.PHONY : src/coap_debug.o

# target to build an object file
src/coap_debug.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_debug.c.o
.PHONY : src/coap_debug.c.o

src/coap_debug.i: src/coap_debug.c.i

.PHONY : src/coap_debug.i

# target to preprocess a source file
src/coap_debug.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_debug.c.i
.PHONY : src/coap_debug.c.i

src/coap_debug.s: src/coap_debug.c.s

.PHONY : src/coap_debug.s

# target to generate assembly for a file
src/coap_debug.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_debug.c.s
.PHONY : src/coap_debug.c.s

src/coap_event.o: src/coap_event.c.o

.PHONY : src/coap_event.o

# target to build an object file
src/coap_event.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_event.c.o
.PHONY : src/coap_event.c.o

src/coap_event.i: src/coap_event.c.i

.PHONY : src/coap_event.i

# target to preprocess a source file
src/coap_event.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_event.c.i
.PHONY : src/coap_event.c.i

src/coap_event.s: src/coap_event.c.s

.PHONY : src/coap_event.s

# target to generate assembly for a file
src/coap_event.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_event.c.s
.PHONY : src/coap_event.c.s

src/coap_hashkey.o: src/coap_hashkey.c.o

.PHONY : src/coap_hashkey.o

# target to build an object file
src/coap_hashkey.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_hashkey.c.o
.PHONY : src/coap_hashkey.c.o

src/coap_hashkey.i: src/coap_hashkey.c.i

.PHONY : src/coap_hashkey.i

# target to preprocess a source file
src/coap_hashkey.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_hashkey.c.i
.PHONY : src/coap_hashkey.c.i

src/coap_hashkey.s: src/coap_hashkey.c.s

.PHONY : src/coap_hashkey.s

# target to generate assembly for a file
src/coap_hashkey.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_hashkey.c.s
.PHONY : src/coap_hashkey.c.s

src/coap_io.o: src/coap_io.c.o

.PHONY : src/coap_io.o

# target to build an object file
src/coap_io.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_io.c.o
.PHONY : src/coap_io.c.o

src/coap_io.i: src/coap_io.c.i

.PHONY : src/coap_io.i

# target to preprocess a source file
src/coap_io.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_io.c.i
.PHONY : src/coap_io.c.i

src/coap_io.s: src/coap_io.c.s

.PHONY : src/coap_io.s

# target to generate assembly for a file
src/coap_io.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_io.c.s
.PHONY : src/coap_io.c.s

src/coap_notls.o: src/coap_notls.c.o

.PHONY : src/coap_notls.o

# target to build an object file
src/coap_notls.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_notls.c.o
.PHONY : src/coap_notls.c.o

src/coap_notls.i: src/coap_notls.c.i

.PHONY : src/coap_notls.i

# target to preprocess a source file
src/coap_notls.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_notls.c.i
.PHONY : src/coap_notls.c.i

src/coap_notls.s: src/coap_notls.c.s

.PHONY : src/coap_notls.s

# target to generate assembly for a file
src/coap_notls.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_notls.c.s
.PHONY : src/coap_notls.c.s

src/coap_openssl.o: src/coap_openssl.c.o

.PHONY : src/coap_openssl.o

# target to build an object file
src/coap_openssl.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_openssl.c.o
.PHONY : src/coap_openssl.c.o

src/coap_openssl.i: src/coap_openssl.c.i

.PHONY : src/coap_openssl.i

# target to preprocess a source file
src/coap_openssl.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_openssl.c.i
.PHONY : src/coap_openssl.c.i

src/coap_openssl.s: src/coap_openssl.c.s

.PHONY : src/coap_openssl.s

# target to generate assembly for a file
src/coap_openssl.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_openssl.c.s
.PHONY : src/coap_openssl.c.s

src/coap_prng.o: src/coap_prng.c.o

.PHONY : src/coap_prng.o

# target to build an object file
src/coap_prng.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_prng.c.o
.PHONY : src/coap_prng.c.o

src/coap_prng.i: src/coap_prng.c.i

.PHONY : src/coap_prng.i

# target to preprocess a source file
src/coap_prng.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_prng.c.i
.PHONY : src/coap_prng.c.i

src/coap_prng.s: src/coap_prng.c.s

.PHONY : src/coap_prng.s

# target to generate assembly for a file
src/coap_prng.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_prng.c.s
.PHONY : src/coap_prng.c.s

src/coap_session.o: src/coap_session.c.o

.PHONY : src/coap_session.o

# target to build an object file
src/coap_session.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_session.c.o
.PHONY : src/coap_session.c.o

src/coap_session.i: src/coap_session.c.i

.PHONY : src/coap_session.i

# target to preprocess a source file
src/coap_session.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_session.c.i
.PHONY : src/coap_session.c.i

src/coap_session.s: src/coap_session.c.s

.PHONY : src/coap_session.s

# target to generate assembly for a file
src/coap_session.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_session.c.s
.PHONY : src/coap_session.c.s

src/coap_tcp.o: src/coap_tcp.c.o

.PHONY : src/coap_tcp.o

# target to build an object file
src/coap_tcp.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_tcp.c.o
.PHONY : src/coap_tcp.c.o

src/coap_tcp.i: src/coap_tcp.c.i

.PHONY : src/coap_tcp.i

# target to preprocess a source file
src/coap_tcp.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_tcp.c.i
.PHONY : src/coap_tcp.c.i

src/coap_tcp.s: src/coap_tcp.c.s

.PHONY : src/coap_tcp.s

# target to generate assembly for a file
src/coap_tcp.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_tcp.c.s
.PHONY : src/coap_tcp.c.s

src/coap_time.o: src/coap_time.c.o

.PHONY : src/coap_time.o

# target to build an object file
src/coap_time.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_time.c.o
.PHONY : src/coap_time.c.o

src/coap_time.i: src/coap_time.c.i

.PHONY : src/coap_time.i

# target to preprocess a source file
src/coap_time.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_time.c.i
.PHONY : src/coap_time.c.i

src/coap_time.s: src/coap_time.c.s

.PHONY : src/coap_time.s

# target to generate assembly for a file
src/coap_time.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/coap_time.c.s
.PHONY : src/coap_time.c.s

src/encode.o: src/encode.c.o

.PHONY : src/encode.o

# target to build an object file
src/encode.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/encode.c.o
.PHONY : src/encode.c.o

src/encode.i: src/encode.c.i

.PHONY : src/encode.i

# target to preprocess a source file
src/encode.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/encode.c.i
.PHONY : src/encode.c.i

src/encode.s: src/encode.c.s

.PHONY : src/encode.s

# target to generate assembly for a file
src/encode.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/encode.c.s
.PHONY : src/encode.c.s

src/mem.o: src/mem.c.o

.PHONY : src/mem.o

# target to build an object file
src/mem.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/mem.c.o
.PHONY : src/mem.c.o

src/mem.i: src/mem.c.i

.PHONY : src/mem.i

# target to preprocess a source file
src/mem.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/mem.c.i
.PHONY : src/mem.c.i

src/mem.s: src/mem.c.s

.PHONY : src/mem.s

# target to generate assembly for a file
src/mem.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/mem.c.s
.PHONY : src/mem.c.s

src/net.o: src/net.c.o

.PHONY : src/net.o

# target to build an object file
src/net.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/net.c.o
.PHONY : src/net.c.o

src/net.i: src/net.c.i

.PHONY : src/net.i

# target to preprocess a source file
src/net.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/net.c.i
.PHONY : src/net.c.i

src/net.s: src/net.c.s

.PHONY : src/net.s

# target to generate assembly for a file
src/net.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/net.c.s
.PHONY : src/net.c.s

src/option.o: src/option.c.o

.PHONY : src/option.o

# target to build an object file
src/option.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/option.c.o
.PHONY : src/option.c.o

src/option.i: src/option.c.i

.PHONY : src/option.i

# target to preprocess a source file
src/option.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/option.c.i
.PHONY : src/option.c.i

src/option.s: src/option.c.s

.PHONY : src/option.s

# target to generate assembly for a file
src/option.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/option.c.s
.PHONY : src/option.c.s

src/pdu.o: src/pdu.c.o

.PHONY : src/pdu.o

# target to build an object file
src/pdu.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/pdu.c.o
.PHONY : src/pdu.c.o

src/pdu.i: src/pdu.c.i

.PHONY : src/pdu.i

# target to preprocess a source file
src/pdu.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/pdu.c.i
.PHONY : src/pdu.c.i

src/pdu.s: src/pdu.c.s

.PHONY : src/pdu.s

# target to generate assembly for a file
src/pdu.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/pdu.c.s
.PHONY : src/pdu.c.s

src/resource.o: src/resource.c.o

.PHONY : src/resource.o

# target to build an object file
src/resource.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/resource.c.o
.PHONY : src/resource.c.o

src/resource.i: src/resource.c.i

.PHONY : src/resource.i

# target to preprocess a source file
src/resource.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/resource.c.i
.PHONY : src/resource.c.i

src/resource.s: src/resource.c.s

.PHONY : src/resource.s

# target to generate assembly for a file
src/resource.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/resource.c.s
.PHONY : src/resource.c.s

src/str.o: src/str.c.o

.PHONY : src/str.o

# target to build an object file
src/str.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/str.c.o
.PHONY : src/str.c.o

src/str.i: src/str.c.i

.PHONY : src/str.i

# target to preprocess a source file
src/str.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/str.c.i
.PHONY : src/str.c.i

src/str.s: src/str.c.s

.PHONY : src/str.s

# target to generate assembly for a file
src/str.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/str.c.s
.PHONY : src/str.c.s

src/subscribe.o: src/subscribe.c.o

.PHONY : src/subscribe.o

# target to build an object file
src/subscribe.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/subscribe.c.o
.PHONY : src/subscribe.c.o

src/subscribe.i: src/subscribe.c.i

.PHONY : src/subscribe.i

# target to preprocess a source file
src/subscribe.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/subscribe.c.i
.PHONY : src/subscribe.c.i

src/subscribe.s: src/subscribe.c.s

.PHONY : src/subscribe.s

# target to generate assembly for a file
src/subscribe.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/subscribe.c.s
.PHONY : src/subscribe.c.s

src/uri.o: src/uri.c.o

.PHONY : src/uri.o

# target to build an object file
src/uri.c.o:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/uri.c.o
.PHONY : src/uri.c.o

src/uri.i: src/uri.c.i

.PHONY : src/uri.i

# target to preprocess a source file
src/uri.c.i:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/uri.c.i
.PHONY : src/uri.c.i

src/uri.s: src/uri.c.s

.PHONY : src/uri.s

# target to generate assembly for a file
src/uri.c.s:
	$(MAKE) -f CMakeFiles/coap-3.dir/build.make CMakeFiles/coap-3.dir/src/uri.c.s
.PHONY : src/uri.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install/strip"
	@echo "... install/local"
	@echo "... coap-3"
	@echo "... coap-client"
	@echo "... coap-rd"
	@echo "... QoS_organizer"
	@echo "... coap-server"
	@echo "... QoS_analyzer"
	@echo "... tiny"
	@echo "... etsi_iot_01"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... list_install_components"
	@echo "... install"
	@echo "... doc_doxygen"
	@echo "... examples/coap-client.o"
	@echo "... examples/coap-client.i"
	@echo "... examples/coap-client.s"
	@echo "... examples/coap-rd.o"
	@echo "... examples/coap-rd.i"
	@echo "... examples/coap-rd.s"
	@echo "... examples/coap-server.o"
	@echo "... examples/coap-server.i"
	@echo "... examples/coap-server.s"
	@echo "... examples/etsi_iot_01.o"
	@echo "... examples/etsi_iot_01.i"
	@echo "... examples/etsi_iot_01.s"
	@echo "... examples/tiny.o"
	@echo "... examples/tiny.i"
	@echo "... examples/tiny.s"
	@echo "... my_libcoap/QoS_analyzer.o"
	@echo "... my_libcoap/QoS_analyzer.i"
	@echo "... my_libcoap/QoS_analyzer.s"
	@echo "... my_libcoap/QoS_organizer.o"
	@echo "... my_libcoap/QoS_organizer.i"
	@echo "... my_libcoap/QoS_organizer.s"
	@echo "... src/address.o"
	@echo "... src/address.i"
	@echo "... src/address.s"
	@echo "... src/async.o"
	@echo "... src/async.i"
	@echo "... src/async.s"
	@echo "... src/block.o"
	@echo "... src/block.i"
	@echo "... src/block.s"
	@echo "... src/coap_asn1.o"
	@echo "... src/coap_asn1.i"
	@echo "... src/coap_asn1.s"
	@echo "... src/coap_cache.o"
	@echo "... src/coap_cache.i"
	@echo "... src/coap_cache.s"
	@echo "... src/coap_debug.o"
	@echo "... src/coap_debug.i"
	@echo "... src/coap_debug.s"
	@echo "... src/coap_event.o"
	@echo "... src/coap_event.i"
	@echo "... src/coap_event.s"
	@echo "... src/coap_hashkey.o"
	@echo "... src/coap_hashkey.i"
	@echo "... src/coap_hashkey.s"
	@echo "... src/coap_io.o"
	@echo "... src/coap_io.i"
	@echo "... src/coap_io.s"
	@echo "... src/coap_notls.o"
	@echo "... src/coap_notls.i"
	@echo "... src/coap_notls.s"
	@echo "... src/coap_openssl.o"
	@echo "... src/coap_openssl.i"
	@echo "... src/coap_openssl.s"
	@echo "... src/coap_prng.o"
	@echo "... src/coap_prng.i"
	@echo "... src/coap_prng.s"
	@echo "... src/coap_session.o"
	@echo "... src/coap_session.i"
	@echo "... src/coap_session.s"
	@echo "... src/coap_tcp.o"
	@echo "... src/coap_tcp.i"
	@echo "... src/coap_tcp.s"
	@echo "... src/coap_time.o"
	@echo "... src/coap_time.i"
	@echo "... src/coap_time.s"
	@echo "... src/encode.o"
	@echo "... src/encode.i"
	@echo "... src/encode.s"
	@echo "... src/mem.o"
	@echo "... src/mem.i"
	@echo "... src/mem.s"
	@echo "... src/net.o"
	@echo "... src/net.i"
	@echo "... src/net.s"
	@echo "... src/option.o"
	@echo "... src/option.i"
	@echo "... src/option.s"
	@echo "... src/pdu.o"
	@echo "... src/pdu.i"
	@echo "... src/pdu.s"
	@echo "... src/resource.o"
	@echo "... src/resource.i"
	@echo "... src/resource.s"
	@echo "... src/str.o"
	@echo "... src/str.i"
	@echo "... src/str.s"
	@echo "... src/subscribe.o"
	@echo "... src/subscribe.i"
	@echo "... src/subscribe.s"
	@echo "... src/uri.o"
	@echo "... src/uri.i"
	@echo "... src/uri.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

