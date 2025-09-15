/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:14:19 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 16:20:48 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_length;
	size_t	src_length;

	i = 0;
	dest_length = ft_strlen(dest);
	src_length = ft_strlen(src);
	if (dest_length >= size)
		return (size + src_length);
	while (i < size - dest_length - 1 && src[i] != 0)
	{
		dest[dest_length + i] = src[i];
		i++;
	}
	dest[dest_length + i] = 0;
	return (src_length + dest_length);
}
/*
#include <stdio.h>

int	main(void)
{
	char	dest[10] = "Hello";

	ft_strlcat(dest, " Worldd", 6);  // Becomes "Hello"
	printf("%s", dest);
	//printf("%d", ft_strlcat(stringd, string, 15));
	return (0);
}
*/