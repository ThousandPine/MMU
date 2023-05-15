CC = gcc

Page_test: Page_test.c VPage.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin