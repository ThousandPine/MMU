CC = gcc

page_test: test/page_test.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

buddy_test: test/buddy_test.c buddy.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

segment_test: test/segment_test.c segment.c buddy.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

memory_test: test/memory_test.c memory.c segment.c buddy.c vpage.c ppage.c
	@$(CC) $^ -o $@.bin
	@./$@.bin

all: mem.c memory.c segment.c buddy.c vpage.c ppage.c
	@$(CC) $^ -o mem
	@./mem

.PHONY: clean

clean:
	@rm -f *.bin mem