/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:07:18 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/20 15:50:55 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	str_count(char const *str, char c)
{
	int	i;
	int	str_count;

	i = 0;
	str_count = 0;
	while (str[i])
	{
		while (str[i] != c && str[i])
			i++;
		if (i > 0)
			str_count++;
		while (str[i] == c && str[i])
			i++;
	}
	return (str_count);
}

static int	ft_strlens(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != c && str[i])
		i++;
	return (i);
}

static int	ft_str(char ***split, int j, char const *str, char c)
{
	int	i;

	i = 0;
	(*split)[j] = malloc(ft_strlens(str, c) + 1);
	if (!(*split)[j])
	{
		while (--j >= 0)
			free((*split)[j]);
		free((*split));
		return (-1);
	}
	while (str[i] && str[i] != c)
	{
		(*split)[j][i] = str[i];
		i++;
	}
	(*split)[j][i] = '\0';
	return (i);
}

char	**ft_split(char const *str, char c)
{
	char	**split;
	int		i;
	int		j;
	int		str_lgth;

	split = malloc(sizeof(char *) * (str_count(str, c) + 1));
	if (split == NULL)
		return (NULL);
	j = 0;
	i = 0;
	str_lgth = 0;
	while (str[i])
	{
		str_lgth = 0;
		while (str[i] == c && str[i])
			i++;
		str_lgth = ft_str(&split, j, &str[i], c);
		if (str_lgth < 0)
			return (NULL);
		if (str_lgth > 0)
			j++;
		i += str_lgth;
	}
	split[j] = NULL;
	return ((char **)split);
}
