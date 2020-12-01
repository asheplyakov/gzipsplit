
CFLAGS ?= -O2 -g -Wall

gzipsplit: gzipsplit.c Makefile
	$(CC) $(CFLAGS) -o $@ $<

check: gzipsplit test/ab.img
	./test/test_ab.sh

test/ab.img: test/a.txt test/b.txt
	gzip -c < test/a.txt > $@.tmp
	gzip -c < test/b.txt >> $@.tmp
	mv $@.tmp $@

clean:
	@rm -f gzipsplit

.PHONY: clean
