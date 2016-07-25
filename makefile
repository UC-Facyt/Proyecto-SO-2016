# Makefile del Proyecto de Sistemas Operativos

CC = gcc
FLAGS = -lpthread

# Test
DIR = bin
BIN = $(DIR)/main

all:	main.c
	@mkdir -p bin
	$(CC) $< -o $(BIN) $(FLAGS)
	@echo "Done."

test:	test.c
	@mkdir -p bin
	$(CC) $< -o $(BIN) $(FLAGS)

clean:
	@rm -rf $(DIR)
	@echo "Borrado!"

love:
	@echo "Not War?"
	