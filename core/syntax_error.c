/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:14:47 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 10:29:01 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(t_entry *entry)
{
	if (entry->next == NULL)
		safe_write(1, "syntax error near unexpected token `newline'\n", 45);
	else
	{
		if (safe_write(1, "syntax error near unexpected token `", 36) == -1)
			return (-1);
		if (safe_write(1, entry->next->raw_entry,
				ft_strlen(entry->next->raw_entry)) == -1)
			return (-1);
		safe_write(1, "'\n", 2);
	}
	return (-1);
}
