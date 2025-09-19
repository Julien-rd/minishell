/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:57:26 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 21:23:30 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int char_value, size_t n)
{
	size_t			i;
	unsigned char	*temp_str;
	unsigned char	c;

	c = (unsigned char)char_value;
	temp_str = (unsigned char *)str;
	i = 0;
	while (i < n)
	{
		if (temp_str[i] == c)
			return (&temp_str[i]);
		i++;
	}
	return (NULL);
}
