.PHONY: tests
tests:
	@ gcc -fno-asm tests.c tests.h linked_list.c linked_list.h -o lab22demo

.PHONY: run-valgrind
run-valgrind: tests
	CK_FORK=no valgrind --quiet --leak-check=full --error-exitcode=1 --show-leak-kinds=all --errors-for-leak-kinds=all ./lab22demo