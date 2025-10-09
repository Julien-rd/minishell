/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:44:13 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 16:00:35 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (to_find[k])
		k++;
	if (!to_find[0])
		return ((char *)str);
	while (i < len && str[i])
	{
		j = 0;
		while (str[i + j] == to_find[j] && str[i + j] && i + j < len)
		{
			j++;
			if (j == k)
				return ((char *)&str[i]);
		}
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>

int	main(void)
{
	char sonny[20] = "";
	char son[21] = "my brmyäd bro humble";
	printf("%s\n", ft_strstr("hello", ""));
	return (0);
}*/