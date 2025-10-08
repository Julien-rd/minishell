/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:56:25 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/26 17:12:17 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*new;

	new = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		new[i] = c;
		i++;
	}
	return (b);
}
/*
#include <stdio.h>
#include <string.h>
int main()
{
    char hello[6];
    hello[0] = 'h';
	hello[1] = 'h';
	hello[2] = 'h';
    printf("%s\n", (char *)ft_memset((void *)hello, 'b', 15));
    return 0;
}*/
