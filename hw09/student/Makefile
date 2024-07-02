# HW9 (Dynamic Memory) - CS2110: Spring 2024
# GCC flags from the syllabus (each flag described for the curious minds!)
# Flag info credit: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

CFLAGS = -std=c99                          # Using the C99 standard
CFLAGS += -Wall                            # This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
CFLAGS += -pedantic                        # Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++
CFLAGS += -Wextra                          # This enables some extra warning flags that are not enabled by -Wall
CFLAGS += -Werror                          # Make all warnings into errors
CFLAGS += -O0															 # Optimize even more. GCC performs nearly all supported optimizations that do not involve a space-speed tradeoff.
CFLAGS += -Wstrict-prototypes              # Warn if a function is declared or defined without specifying the argument types
CFLAGS += -Wold-style-definition           # Warn if an old-style function definition is used. A warning is given even if there is a previous prototype

STUDENTCFLAGS = -g                         # Generate debugging information
STUDENTCFLAGS += -fcf-protection=none      # Workaround for a bug that can make debugging annoying: https://stackoverflow.com/questions/64697087/gdb-shows-incorrect-arguments-of-functions-for-stack-frames
STUDENTCFLAGS += -funsigned-char					 # Make plain char type unsigned to curb gradescope towards intended behavior

# Source files to be compiled together (for local command line testing)
CFILES = main.c slack.c
HFILES = slack.h suites/test_utils.h

# Executable name
OBJNAME = hw9

# Source files for the autograder (skipping main, cause check has it's own main method)
AUTOGRADER_C_FILES = slack.c

# Uses pkg-config to retrieve package information about check -- Unit Testing Framework
CHECK_LIBS = $(shell pkg-config --cflags --libs check)
# If CS2110_AUTOGRADER_VERSION exists, we are running in a autograder Docker container which does not have architecture-specfic obj files, so just put a . (current directory) instead of the architecture
# If it does not, then we need to get the current architecture using uname
ARCH = $(if $(CS2110_AUTOGRADER_VERSION),.,$(shell uname -m))

# Locate compiled files based on system architecture
TEST_FILES_HIDDEN = suites/$(ARCH)/test_entry.o suites/$(ARCH)/test_utils.o
TEST_FILES_VISIBLE = suites/slack_suite.c

#########################
# GENERAL BUILD TARGETS #
#########################

.PHONY: default
default: run-tests

# Use gcc to strip out comments from the code
# grep will return 1 if no match is found, so we NOT the output
# Also compile student files with poison to make sure they're not using illegal functions
.PHONY: check-violations
check-violations: $(AUTOGRADER_C_FILES)
	@ gcc -fsyntax-only -fno-asm $(CFLAGS) $(CSTUDENTFLAGS) -include suites/poison.h $(AUTOGRADER_C_FILES)

# Compile all source files with the given flags into the specified executable object!
hw9: $(CFILES) check-violations
	@ gcc -fno-asm $(CFLAGS) $(CFILES) -o $(OBJNAME)

# Remove all sort of object files and executables
.PHONY: clean
clean:
	@ rm -f $(OBJNAME) tests suites/*.o *.o *.out

#######################
# AUTOGRADING TARGETS #
#######################

# Testing with check library
.PHONY: tests
tests: $(AUTOGRADER_C_FILES) $(TEST_FILES_HIDDEN) $(TEST_FILES_VISIBLE) check-violations
	@ gcc -fno-asm -include suites/fakemalloc.h $(CFLAGS) $(STUDENTCFLAGS) $(AUTOGRADER_C_FILES) $(TEST_FILES_HIDDEN) $(TEST_FILES_VISIBLE) $(CHECK_LIBS) -o tests

.PHONY: run-tests
run-tests: tests
	@ ./tests

# To run a specific test case (or all similar to tests)
.PHONY: run-case
run-case: tests
	@ ./tests $(TEST)

# To run gdb on testcase(s)
.PHONY: run-gdb
run-gdb: tests
	@ CK_FORK=no gdb --args ./tests $(TEST)

# To prevent leak false postives, tell check not to fork.
# See: http://check.sourceforge.net/doc/check_html/check_4.html#Finding-Memory-Leaks
.PHONY: run-valgrind
run-valgrind: tests
	CK_FORK=no valgrind --quiet --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all ./tests $(TEST)

#################################################
# Internal targets (not needed for student use) #
#################################################
%.o: %.c $(HFILES)
	gcc $(CFLAGS) -c $< -o $@

# Generate all the .o files needed for the grader as well as a list of all the test files
testfiles.out: $(subst $(ARCH)/,,$(TEST_FILES_HIDDEN))
	@ echo "#!/bin/bash" > testfiles.out
	@ echo "TEST_FILES_HIDDEN=($(notdir $(TEST_FILES_HIDDEN)))" >> testfiles.out
	@ echo "TEST_FILES_VISIBLE=($(notdir $(TEST_FILES_VISIBLE)))" >> testfiles.out
	@ chmod u+x testfiles.out
