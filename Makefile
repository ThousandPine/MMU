CC = gcc

PPage_test: PPage_test.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin