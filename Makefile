CC = gcc

test_PPage: test.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin