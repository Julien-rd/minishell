/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 06:43:08 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 12:52:43 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	*ft_free(char **arr)
{
	if (!(*arr))
		return (NULL);
	free(*arr);
	*arr = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*arr;
	char		*next_line;
	int			index;

	next_line = NULL;
	if (fd < 0)
		return (free(arr), arr = NULL, NULL);
	arr = ft_read(arr, fd);
	index = ft_next_line_check(arr, 0);
	if (index == 0)
	{
		next_line = ft_gnlstrjoin(next_line, arr, 1);
		arr = ft_free(&arr);
		return (next_line);
	}
	if (index == -1)
		return (NULL);
	next_line = ft_gnlstrjoin(next_line, arr, 1);
	if (!next_line)
		return (ft_free(&arr));
	arr = ft_trim(arr, index);
	if (!arr)
		return (ft_free(&next_line));
	return (next_line);
}
