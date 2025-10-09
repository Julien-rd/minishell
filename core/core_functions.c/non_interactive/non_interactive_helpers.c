/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:43:40 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 15:03:02 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cut_nl(char *buf)
{
	size_t	len;

	len = ft_strlen(buf);
	if (len == 0)
		return ;
	buf[len - 1] = '\0';
}
