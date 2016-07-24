# Makefile del Proyecto de Sistemas Operativos

CC = gcc
FLAGS = -Wall -Wextra -lpthread

# Test
DIR = bin
BIN = $(DIR)/test

all:	test
	@echo "Done."

test:	test.c
	@mkdir -p bin
	$(CC) $< -o $(BIN) $(FLAGS)

clean:
	@rm -rf $(DIR)
	@echo "Borrado!"

love:
	@echo "Not Warcraft?"
	