/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:17:51 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 21:22:32 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	s_lgth;
	char	*strtrim;

	s_lgth = ft_strlen(s1);
	start = 0;
	end = s_lgth;
	if (!s_lgth)
		return (ft_strdup(""));
	while (s1[start] && ft_strchr(set, s1[start]) != NULL)
		start++;
	while (end > 0 && ft_strchr(set, s1[end - 1]) != NULL)
		end--;
	if (start >= end)
		return (ft_strdup(""));
	strtrim = malloc(sizeof(char) * (end - start + 1));
	if (!strtrim)
		return (NULL);
	ft_strlcpy(strtrim, &s1[start], end - start + 1);
	return (strtrim);
}
