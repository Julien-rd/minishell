/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 20:17:42 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/25 17:55:47 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			s_lgth;
	char			*new_str;
	unsigned int	i;

	i = -1;
	s_lgth = ft_strlen(s);
	if (!s_lgth)
		return (ft_strdup(""));
	new_str = malloc(sizeof(char) * (s_lgth + 1));
	if (!new_str)
		return (NULL);
	while (s[++i])
		new_str[i] = (*f)(i, s[i]);
	new_str[i] = '\0';
	return (new_str);
}
