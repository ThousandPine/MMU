CC = gcc

page_test: page_test.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

buddy_test: buddy_test.c buddy.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin