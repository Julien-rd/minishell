/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:48:53 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 13:53:01 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_next_line_check(char *str, int specifier)
{
	int	i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
	{
		if (str[i] == '\n')
			return (i + 1);
	}
	if (specifier == 1)
		return (i);
	return (0);
}

size_t	ft_strlen(const char *s, int specifier)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (specifier == 1 && s[i] == '\n')
			return (i);
		i++;
	}
	return (i);
}

char	*ft_gnlstrjoin(char const *s1, char const *s2, int spec)
{
	int		i;
	int		k;
	char	*ptr;

	i = -1;
	k = -1;
	ptr = malloc(ft_strlen((char *)s1, spec) + ft_strlen((char *)s2, spec) + 2);
	if (ptr == NULL)
		return (NULL);
	if (s1)
		while (s1[++i])
			ptr[i] = s1[i];
	i -= (i != -1);
	while (s2[++k])
	{
		ptr[++i] = s2[k];
		if (s2[k] == '\n' && spec == 1)
		{
			ptr[++i] = 0;
			return (ptr);
		}
	}
	free((char *)s1);
	ptr[++i] = 0;
	return (ptr);
}

char	*ft_read(char *arr, int fd)
{
	int		bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	if (ft_next_line_check(arr, 0) > 0)
		return (arr);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		if (arr)
			return (arr);
		return (NULL);
	}
	while (bytes_read > 0)
	{
		buffer[bytes_read] = 0;
		arr = ft_gnlstrjoin(arr, buffer, 0);
		if (ft_next_line_check(arr, 0))
			return (arr);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0)
			return (arr);
	}
	return (arr);
}

char	*ft_trim(char *arr, int index)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = (char *)malloc(sizeof(char) * ft_strlen(&arr[index], 0) + 1);
	if (!ptr)
	{
		free(arr);
		return (NULL);
	}
	while (arr[index + i] != '\0')
	{
		ptr[i] = arr[index + i];
		i++;
	}
	ptr[i] = 0;
	free(arr);
	return (ptr);
}
