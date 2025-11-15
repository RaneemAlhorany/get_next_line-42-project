/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralhoura <ralhoura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:25:55 by ralhoura          #+#    #+#             */
/*   Updated: 2025/10/07 18:52:02 by ralhoura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*free_chunk(char **chunk)
{
	if (chunk && *chunk)
	{
		free(*chunk);
		*chunk = NULL;
	}
	return (NULL);
}

char	*extract_line(char **chunk)
{
	char	*line;
	size_t	index;

	if (!chunk || !*chunk || !**chunk)
		return (NULL);
	index = 0;
	while ((*chunk)[index] && (*chunk)[index] != '\n')
		index++;
	if ((*chunk)[index] == '\n')
		index++;
	line = malloc(index + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, *chunk, index);
	line[index] = '\0';
	return (line);
}

char	*update_chunk(char **chunk)
{
	char	*new_chunk;
	char	*data_after_nl;
	size_t	len;

	if (!chunk || !*chunk || !**chunk)
		return (NULL);
	data_after_nl = ft_strchr(*chunk, '\n');
	if (data_after_nl)
		data_after_nl++;
	else
		return (free_chunk(chunk));
	if (!*data_after_nl)
		return (free_chunk(chunk));
	len = ft_strlen(data_after_nl);
	new_chunk = malloc(len + 1);
	if (!new_chunk)
		return (free_chunk(chunk));
	ft_memcpy(new_chunk, data_after_nl, len);
	new_chunk[len] = '\0';
	free(*chunk);
	*chunk = NULL;
	return (new_chunk);
}

int	read_and_store(char **chunk, int fd, char *buffer)
{
	char	*temp;
	ssize_t	bytes;

	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		temp = ft_strjoin(*chunk, buffer);
		if (!temp)
		{
			free(buffer);
			return (-1);
		}
		free(*chunk);
		*chunk = temp;
		if (ft_strchr(*chunk, '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes < 0)
		return (-1);
	return (bytes > 0 && (*chunk && **chunk));
}

char	*get_next_line(int fd)
{
	static char	*chunk[1024];
	char		*line;
	char		*buffer;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (read_and_store(&chunk[fd], fd, buffer) == -1)
		return (free_chunk(&chunk[fd]));
	if (!chunk[fd] || !*chunk[fd])
		return (free_chunk(&chunk[fd]));
	line = extract_line(&chunk[fd]);
	if (!line)
		return (free_chunk(&chunk[fd]));
	chunk[fd] = update_chunk(&chunk[fd]);
	return (line);
}
