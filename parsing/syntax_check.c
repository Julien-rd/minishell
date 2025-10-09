/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:14:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 14:49:44 by jromann          ###   ########.fr       */
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

static int	op_match(int operator, t_entry * entry)
{
	if (entry->spec == operator && (entry->next == NULL
			|| entry->next->spec != operator))
		return (0);
	return (1);
}

int	syntax_check(t_sh *sh)
{
	t_entry	*entry;

	entry = sh->entries;
	if (entry->spec == PIPE)
		return (syntax_error(entry));
	while (entry != NULL)
	{
		if (!op_match(entry->spec, entry))
			return (syntax_error(entry));
		entry = entry->next;
	}
	return (0);
}
