//! command

// cc -Wall -Wextra -Werror -g \
// main.c get_next_line.c get_next_line_utils.c \
// -D BUFFER_SIZE=1

// valgrind --leak-check=full --show-leak-kinds=all ./a.out

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

void	test_single_fd(char *filename)
{
	int		fd;
	char	*line;
	int		i;

	printf("\n===== SINGLE FD TEST (%s) =====\n", filename);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return;
	}
	i = 1;
	while ((line = get_next_line(fd)))
	{
		printf("Line %d: [%s]", i++, line);
		free(line);
	}
	close(fd);
}

void	test_invalid_fd(void)
{
	printf("\n===== INVALID FD TEST =====\n");
	if (get_next_line(-1) == NULL)
		printf("OK: returned NULL for fd = -1\n");
	else
		printf("ERROR: did not return NULL for fd = -1\n");
}

void	test_multiple_fd(char *file1, char *file2)
{
	int		fd1;
	int		fd2;
	char	*line;

	printf("\n===== MULTIPLE FD TEST =====\n");
	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);

	line = get_next_line(fd1);
	printf("fd1: %s", line);
	free(line);

	line = get_next_line(fd2);
	printf("fd2: %s", line);
	free(line);

	line = get_next_line(fd1);
	printf("fd1: %s", line);
	free(line);

	line = get_next_line(fd2);
	printf("fd2: %s", line);
	free(line);

	close(fd1);
	close(fd2);
}

void	test_closed_fd(char *filename)
{
	int		fd;

	printf("\n===== CLOSED FD TEST =====\n");
	fd = open(filename, O_RDONLY);
	close(fd);
	if (get_next_line(fd) == NULL)
		printf("OK: returned NULL for closed fd\n");
	else
		printf("ERROR: did not handle closed fd properly\n");
}

int	main(void)
{
	test_single_fd("tests/empty.txt");
	test_single_fd("tests/one_line.txt");
	test_single_fd("tests/no_newline.txt");
	test_single_fd("tests/multiple_lines.txt");
	test_single_fd("tests/only_newlines.txt");
	test_single_fd("tests/long_line.txt");

	test_invalid_fd();
	test_closed_fd("tests/one_line.txt");
	test_multiple_fd("tests/multiple_lines.txt", "tests/no_newline.txt");

	printf("\n===== END OF TESTS =====\n");
	return (0);
}





tests/
 ├── empty.txt
 ├── one_line.txt
 ├── no_newline.txt
 ├── multiple_lines.txt
 ├── only_newlines.txt
 └── long_line.txt


