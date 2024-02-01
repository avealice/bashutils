CC=gcc
FLAGS = -Wall -Wextra -Werror

all : cat

s21_cat : cat.o
	@echo "======="
	@echo "LINKING"
	@echo "======="
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "===="
	@echo "DONE"
	@echo "===="

s21_cat.o : cat.c cat.h
	@$(CC) $(CFLAGS) -c $^

tests : cat
	@bash ./tests.sh
	@rm -f log.txt

clean :
	@echo "========"
	@echo "CLEANING"
	@echo "========"
	@rm -f *.o *.gch
	@rm -f cat
	@echo "===="
	@echo "DONE"
	@echo "===="

rebuild :
	$(MAKE) clean
	$(MAKE) all