#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void switch_file(FILE **current, int *count) {
	char buf[sizeof("s_1234567890.gz")];
	memset(buf, 0, sizeof(buf));
	if (*current) {
		if (fflush(*current) != 0)
			exit(2);
		if (fclose(*current) != 0)
			exit(3);
		*current = NULL;
	}
	sprintf(buf, "s_%d.gz", *count);
	++(*count);
	*current = fopen(buf, "w");
	if (!*current)
		exit(5);
}


int main(int argc, char **argv) {
	char buf[4];
	const char magic[3] = "\x1F\x8B\x08";
	const char flg_mask = 0x1f;
	size_t nread;
	int count = 0;
	FILE* current = NULL;
	FILE* in;
	in = fopen(argv[1], "r");
	if (!in) {
		exit(7);
	}

	while (!feof(in)) {
		nread = fread(buf, sizeof(buf), 1, in);
		if (nread < 1) {
			fwrite(buf, sizeof(buf), 1, current);
			continue;
		}
		if (!current) {
			switch_file(&current, &count);
		} else if (strncmp(buf, magic, sizeof(magic)) == 0) {
			if ((buf[3] & ~flg_mask) == 0) {
				printf("switching file %d\n", count);
				switch_file(&current, &count);
			}
		}
		fwrite(buf, sizeof(buf), 1, current);
	}
	fflush(current);
	fclose(current);
	return 0;
}

