/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:42 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:46:25 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*duplicate;
	size_t	src_length;

	src_length = ft_strlen(src);
	duplicate = malloc(src_length + 1);
	if (duplicate == NULL)
		return (NULL);
	ft_strlcpy(duplicate, src, src_length + 1);
	return (duplicate);
}
