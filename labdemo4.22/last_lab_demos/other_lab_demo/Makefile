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
CFILES = main.c 

# Executable name
OBJNAME = demo

# Testing with check library
.PHONY: tests
tests:
	@ gcc -fno-asm $(CFLAGS) $(STUDENTCFLAGS) $(CFILES) -o tests

.PHONY: run-gdb
run-gdb: tests
	@ CK_FORK=no gdb --args ./tests $(TEST)

# To prevent leak false postives, tell check not to fork.
# See: http://check.sourceforge.net/doc/check_html/check_4.html#Finding-Memory-Leaks
.PHONY: run-valgrind
run-valgrind: tests
	CK_FORK=no valgrind --quiet --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all ./tests $(TEST)
