/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:17:50 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:54:09 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int char_value)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = NULL;
	while (str[i])
	{
		if (str[i] == char_value)
			new_str = (char *)&str[i];
		i++;
	}
	if (str[i] == char_value)
		return ((char *)&str[i]);
	else if (new_str == NULL)
		return (NULL);
	else
		return (new_str);
}
/*
#include <stdio.h>

int	main(void)
{
	printf("%s", ft_strchr("hi boy what are u up to", 117));
	return (0);
}*/