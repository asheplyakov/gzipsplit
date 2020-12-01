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

static int can_be_flg(char c) {
	const char flg_mask = 0x1f;
	if ((c & ~flg_mask) == 0)
		return 1;
	else
		return 0;
}

static void process(FILE *in) {
	int count = 0;
	FILE* current = NULL;
	int c1, c2, c3, c4;
	size_t off = 0;
	while (1) {
		c1 = c2 = c3 = c4 = EOF;

		c1 = fgetc(in);
		if (c1 == EOF)
			break;
		off++;
		if (!current) {
			printf("writing file #1\n");
			switch_file(&current, &count);
			if (fputc(c1, current) == EOF)
				exit(11);
			continue;
		}

		if ((char)c1 == '\x1F') {
			c2 = fgetc(in);
			if (c2 == EOF) {
				if (fputc(c1, current) == EOF)
					exit(11);
				break;
			}
			off++;
			if ((char)c2 == '\x8B') {
				c3 = getc(in);
				if (c3 == EOF) {
					if (fputc(c1, current) == EOF)
						exit(11);
					if (fputc(c2, current) == EOF)
						exit(11);
					break;
				}
				off++;
				if ((char)c3 == '\x08') {
					c4 = getc(in);
					if (c4 == EOF) {
						if (fputc(c1, current) == EOF)
							exit(11);
						if (fputc(c2, current) == EOF)
							exit(11);
						if (fputc(c3, current) == EOF)
							exit(11);
						break;
					}
					off++;
					if (can_be_flg((char)c4)) {
						printf("writing file #%d\n", count + 1);
						switch_file(&current, &count);
					}
					if (fputc(c1, current) == EOF)
						exit(11);
					if (fputc(c2, current) == EOF)
						exit(11);
					if (fputc(c3, current) == EOF)
						exit(11);
					if (fputc(c4, current) == EOF)
						exit(11);
				} else {
					/* c3 != '\x08' */
					if (fputc(c1, current) == EOF)
						exit(11);
					if (fputc(c2, current) == EOF)
						exit(11);
					if (fputc(c3, current) == EOF)
						exit(11);
				}
			} else {
				/* c2 != '\x8B' */
				if (fputc(c1, current) == EOF)
					exit(11);
				if (fputc(c2, current) == EOF)
					exit(11);
			}
		} else {
			/* c1 != '\x1F' */
			if (fputc(c1, current) == EOF)
				exit(11);
		}
	}
	if (current) {
		fflush(current);
		fclose(current);
		current = NULL;
	}
}

int main(int argc, char **argv) {
	FILE* in;
	in = fopen(argv[1], "r");
	if (!in) {
		exit(7);
	}
	process(in);
	fclose(in);
	return 0;
}

