CC = gcc

PPage_test: PPage_test.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

VPage_test: VPage_test.c VPage.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin