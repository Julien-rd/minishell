/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:58:19 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/26 18:04:11 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*temp_dst;
	unsigned char	*temp_src;
	size_t			i;

	i = 0;
	temp_dst = (unsigned char *)dst;
	temp_src = (unsigned char *)src;
	while (i < n)
	{
		*(temp_dst++) = *(temp_src++);
		i++;
	}
	return (dst);
}
