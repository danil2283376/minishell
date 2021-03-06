/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melisha <melisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 09:23:48 by melisha           #+#    #+#             */
/*   Updated: 2021/03/18 11:59:44 by melisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libminishell.h"

int		get_next_line(char **line, int fd)
{
	int		i;
	char	*buf;
	int		j;

	i = 0;
	if (!(buf = (char *)malloc(sizeof(char) * 10000)))
		fn_error("no memory allocate");
	while ((j = read(fd, &buf[i], 1)) >= 0 && buf[i] != '\n')
	{
		if (j == 0)
		{
			write(2, "  \b\b", 4);
			if (i == 0)
			{
				write(2, "exit\n", 5);
				exit(errno);
			}
			i--;
		}
		i++;
	}
	buf[i] = ' ';
	buf[i + 1] = '\0';
	*line = buf;
	return (j);
}
