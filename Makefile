CC = gcc

VPage_test: VPage_test.c VPage.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin