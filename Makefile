CC = gcc

Page_test: Page_test.c VPage.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

Buddy_test: Buddy_test.c Buddy.c VPage.c PPage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin