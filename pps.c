/*
 * Prototype Processor aSsembler
 *
 * Computer architecture is by qubane
 * Assembler is by nakidai and released in public domain
 */

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#define LINESZ 128

#define is(x, y) if (!strcmp((x), (y)))

char line[LINESZ];

void parsefile(const char *);

void emit(char inst)
{
	printf("%04b\n", (int)inst);
}

void tolowers(char *s)
{
	for (; *s; ++s)
		*s = tolower(*s);
}

int parsereg(const char *name)
{
	switch (*name)
	{
	case 'b': return 0;
	case 'm': return 1;
	case 'p': return 2;
	default:  return -1;
	}
}

void parseline(char *line, const char *path, size_t n)
{
	char *arg = strchr(line, ' '),
	     *end = strchr(line, '\n');

	arg && (*arg = 0);
	end && (*end = 0);

	if (!*line)
		return;

	tolowers(line);

	if (!arg++)
		is (line, "ca")       emit(8);
		else is (line, "add") emit(9);
		else is (line, "sub") emit(10);
		else is (line, "and") emit(11);
		else is (line, "or")  emit(12);
		else is (line, "xor") emit(13);
		else is (line, "rd")  emit(14);
		else is (line, "wt")  emit(15);
		else is (line, ";")   ;
		else errx(1, "%s:%d: invalid instruction: %s", path, n, line);
	else
		is (line, "lda")
			emit(atoi(arg));
		else is (line, "movc")
			if (tolowers(arg), parsereg(arg) < 0)
				errx(1, "%s:%lu: invalid instruction: %s %s", path, n, line, arg);
			else
				emit(4 + parsereg(arg));
		else is (line, "mov")
			if (tolowers(arg), parsereg(arg) != 0)
				errx(1, "%s:%lu: invalid instruction: %s %s", path, n, line, arg);
			else
				emit(7);
		else is (line, "inc") parsefile(arg);
		else is (line, ";");
		else errx(1, "%s:%lu invalid instruction: %s %s", path, n, line, arg);
}

void parsefile(const char *file)
{
	FILE *fp = fopen(file, "r");
	size_t n = 0;

	if (!fp)
		err(1, "fopen(%s)", file);

	while (fgets(line, sizeof(line), fp))
		parseline(line, file, n++);

	if (ferror(fp))
		err(1, "fgets(%s)", file);
}

int main(int argc, char **argv)
{
	if (!argv[1] || !*argv[1])
		return fprintf(stderr, "usage: %s <filename>\n", *argv), 1;
	parsefile(argv[1]);
}
