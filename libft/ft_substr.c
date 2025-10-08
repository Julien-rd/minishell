/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:04:21 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:52:38 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;
	size_t	s_length;

	i = 0;
	s_length = ft_strlen(s);
	if (start > s_length)
		return (ft_strdup(""));
	substr = malloc(sizeof(char) * len + 1);
	if (substr == NULL)
		return (NULL);
	while (s[start + i] && i < len)
	{
		substr[i] = (char)s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
