CC=gcc
FLAGS = -Wall -Wextra -Werror

all : grep

s21_grep : grep.o
	@echo "======="
	@echo "LINKING"
	@echo "======="
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "===="
	@echo "DONE"
	@echo "===="

s21_grep.o : grep.c grep.h
	@$(CC) $(CFLAGS) -c $^

tests : grep
	@bash ./tests.sh
	@rm -f log.txt

clean :
	@echo "========"
	@echo "CLEANING"
	@echo "========"
	@rm -f *.o *.gch
	@rm -f grep
	@echo "===="
	@echo "DONE"
	@echo "===="

rebuild :
	$(MAKE) clean
	$(MAKE) all