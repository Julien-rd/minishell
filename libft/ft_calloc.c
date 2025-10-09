/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:53:15 by eprottun          #+#    #+#             */
/*   Updated: 2025/05/03 16:03:45 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tmp;
	size_t			i;
	size_t			total_size;

	i = 0;
	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	total_size = count * size;
	tmp = malloc(total_size);
	if (!tmp)
		return (tmp);
	while (i < total_size)
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp);
}
