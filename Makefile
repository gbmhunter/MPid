#
# @file 		Makefile
# @author 		Geoffrey Hunter <gbmhunter@gmail.com> (wwww.cladlab.com)
# @edited 		n/a
# @date 		2013/08/29
# @brief 		Makefile for Linux-based make, to compile Pid library and run unit test code.
# @details
#				See README.rst

# Define the compiler to use (e.g. gcc, g++)
CC = g++

# Define any compile-time flags
CFLAGS = -Wall -g

# Define any directories containing header files other than /usr/include
#
INCLUDES = -I./src/include

# Define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L./test/UnitTest++

# Define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lUnitTest++

# ---------------------
# SOURCE CODE VARIABLES
# ---------------------

# Create a list of C++ files
SRC_CPP_FILES = $(wildcard src/*.cpp)
# Create a list of object files
SRC_OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_CPP_FILES))
# Create a list of dependency files
SRC_DEP_FILES = $(SRC_CPP_FILES:.cpp=.d)
# Linker flags
SRC_LD_FLAGS :=
# Compiler flags 
SRC_CC_FLAGS := -Wall -g

#--------------------
# TEST CODE VARIABLES
#--------------------

TEST_OBJ_FILES := $(patsubst %.cpp,%.o,$(wildcard test/*.cpp))
TEST_LD_FLAGS := 
TEST_CC_FLAGS := -Wall -g

# Phony target
.PHONY: depend clean

# Compile all
all: PidLib Test
	
	# Run Pid unit tests:
	@./test/PidTest.elf
	
PidLib : $(SRC_OBJ_FILES)
	# Make Pid library
	ar r libPid.a $(SRC_OBJ_FILES)
	
# Generic rule for src object files
src/%.o: src/%.cpp
	# Compiling source and making dependency files
	g++ $(SRC_CC_FLAGS) -c -MD -o $@ $<

# Include Makefile code in source dependency files
# -include is like include but shows no errors/warnings
# THIS LINE MUST BE BELOW THE GENERIC RULE FOR SRC OBJECT FILES
-include $(SRC_DEP_FILES)
	
# Compiles unit test code
Test : $(TEST_OBJ_FILES) | PidLib UnitTestLib
	# Compiling unit test code
	g++ $(TEST_LD_FLAGS) -o ./test/PidTest.elf $(TEST_OBJ_FILES) -L./test/UnitTest++ -lUnitTest++ -L./ -lPid
	
# Generic rule for test object files
test/%.o: test/%.cpp
	g++ $(TEST_CC_FLAGS) -c -o $@ $<

# Rule for compiling UnitTest++ library.
UnitTestLib:
	# Compile UnitTest++ library (has it's own Makefile)
	$(MAKE) -C ./test/UnitTest++/ all

clean:
	# Clean UnitTest++ library (has it's own Makefile)
	$(MAKE) -C ./test/UnitTest++/ clean
	
	# Clean everything else
	@echo " Cleaning src object files..."; $(RM) ./src/*.o
	@echo " Cleaning src dependency files..."; $(RM) ./src/*.d
	@echo " Cleaning Pid static library..."; $(RM) ./*.a
	@echo " Cleaning test object files..."; $(RM) ./test/*.o
	@echo " Cleaning test executable..."; $(RM) ./test/*.elf
	
clean-ut:
	# Cleans unit test files
	@echo " Cleaning test object files..."; $(RM) ./test/*.o
	@echo " Cleaning test executable..."; $(RM) ./test/*.elf
	
clean-pid:
	# Cleans all PID code
	@echo " Cleaning src object files..."; $(RM) ./src/*.o
	@echo " Cleaning src dependency files..."; $(RM) ./src/*.d
	@echo " Cleaning Pid static library..."; $(RM) ./*.a
	@echo " Cleaning test object files..."; $(RM) ./test/*.o
	@echo " Cleaning test executable..."; $(RM) ./test/*.elf