/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:57:38 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 19:14:43 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	tmp_dst = (unsigned char *)dst;
	tmp_src = (unsigned char *)src;
	if (tmp_src < tmp_dst)
	{
		while (len > 0)
		{
			len--;
			tmp_dst[len] = tmp_src[len];
		}
		return (dst);
	}
	while (len > 0)
	{
		len--;
		*tmp_dst++ = *tmp_src++;
	}
	return (dst);
}
/*
#include <stdio.h>

int	main(void)
{
	char	word[] = "hello";
	char	*huh = "hollo";

	printf("%s\n", (char*)ft_memmove((void*)huh, (void *)word, 3));
	return (0);
}*/
