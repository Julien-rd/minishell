/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:13:50 by jromann           #+#    #+#             */
/*   Updated: 2025/09/17 12:17:23 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t iter;

	if (!s1 || !s2)
		return (0);
	while (s1[iter] == s2[iter] && s1[iter] != '\0' && s2[iter] != '\0')
		iter++;
	if (s1[iter] == '\0' && s2[iter] == '\0')
		return (1);
	return (0);
}