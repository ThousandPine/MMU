CC = gcc

PPage_test: test.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin