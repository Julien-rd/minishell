/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:14:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 17:26:19 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_error(t_entry *entry, t_sh *sh)
{
	sh->exit_code = 2;
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

int	syntax_check(t_sh *sh)
{
	t_entry	*entry;

	entry = sh->entries;
	while (entry != NULL)
	{
		if (entry->spec == PIPE && (entry->next == NULL
				|| entry->next->spec == PIPE || entry == sh->entries))
			return (syntax_error(entry, sh));
		if (entry->spec == INFILE_OP && (entry->next == NULL
				|| entry->next->spec != INFILE))
			return (syntax_error(entry, sh));
		if (entry->spec == HERE_DOC_OP && (entry->next == NULL
				|| entry->next->spec != HERE_DOC))
			return (syntax_error(entry, sh));
		if (entry->spec == OUTFILE_OP && (entry->next == NULL
				|| entry->next->spec != OUTFILE))
			return (syntax_error(entry, sh));
		if (entry->spec == APPEND_OP && (entry->next == NULL
				|| entry->next->spec != APPEND_FILE))
			return (syntax_error(entry, sh));
		entry = entry->next;
	}
	return (0);
}
