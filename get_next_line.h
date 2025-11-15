/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralhoura <ralhoura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:27:26 by ralhoura          #+#    #+#             */
/*   Updated: 2025/10/07 19:19:33 by ralhoura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

char	*get_next_line(int fd);
char	*free_chunk(char **chunk);
char	*extract_line(char **chunk);
char	*update_chunk(char **chunk);

int		read_and_store(char **chunk, int fd, char *buffer);

size_t	ft_strlen(const char *s);

char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);

void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
