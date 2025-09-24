/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:53:09 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:45:44 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_bzero(void *b, size_t len)
{
	size_t			i;
	unsigned char	*new;

	new = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		new[i] = 0;
		i++;
	}
	return (b);
}
