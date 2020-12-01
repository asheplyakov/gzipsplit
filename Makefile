
CFLAGS ?= -O2 -g -Wall

gzipsplit: gzipsplit.c Makefile
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@rm -f gzipsplit

.PHONY: clean
