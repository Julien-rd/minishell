/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:09:58 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:44:12 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int char_value)
{
	str -= 1;
	while (*(++str))
		if (*str == char_value)
			return ((char *)str);
	if (char_value == 0)
		return ((char *)str);
	return (NULL);
}
