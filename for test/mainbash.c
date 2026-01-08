#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

static void	print_line(char *line)
{
	if (!line)
		printf("[NULL]\n");
	else
	{
		printf("[%s]", line);
		free(line);
	}
}

void	test_eof(char *file)
{
	int fd;
	char *line;

	printf("\n== EOF TEST ==\n");
	fd = open(file, O_RDONLY);
	while ((line = get_next_line(fd)))
		print_line(line);
	line = get_next_line(fd);
	if (line != NULL)
		printf("❌ ERROR: returned value after EOF\n");
	else
		printf("✅ OK: NULL after EOF\n");
	close(fd);
}

void	test_multiple_fd(char *f1, char *f2)
{
	int fd1, fd2;

	printf("\n== MULTIPLE FD TEST ==\n");
	fd1 = open(f1, O_RDONLY);
	fd2 = open(f2, O_RDONLY);

	print_line(get_next_line(fd1));
	print_line(get_next_line(fd2));
	print_line(get_next_line(fd1));
	print_line(get_next_line(fd2));

	close(fd1);
	close(fd2);
}

void	test_invalid_fd(void)
{
	printf("\n== INVALID FD TEST ==\n");
	if (get_next_line(-1) == NULL)
		printf("✅ OK: fd -1 handled\n");
	else
		printf("❌ ERROR: fd -1 not handled\n");
}

int	main(void)
{
	test_eof("tests/no_newline.txt");
	test_multiple_fd("tests/multiple_lines.txt", "tests/one_line.txt");
	test_invalid_fd();
	return (0);
}
