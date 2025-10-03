/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:14:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/03 12:52:32 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_sh *sh)
{
	size_t	iter;
	t_entry	*entry;

	entry = sh->entries;
	iter = 0;
	while (entry != NULL)
	{
		if (entry->spec == PIPE && (entry->next == NULL
				|| entry->next->spec == PIPE || entry == sh->entries))
			return (syntax_error(entry));
		if (entry->spec == INFILE_OP && (entry->next == NULL
				|| entry->next->spec != INFILE))
			return (syntax_error(entry));
		if (entry->spec == HERE_DOC_OP && (entry->next == NULL
				|| entry->next->spec != HERE_DOC))
			return (syntax_error(entry));
		if (entry->spec == OUTFILE_OP && (entry->next == NULL
				|| entry->next->spec != OUTFILE))
			return (syntax_error(entry));
		if (entry->spec == APPEND_OP && (entry->next == NULL
				|| entry->next->spec != APPEND_FILE))
			return (syntax_error(entry));
		entry = entry->next;
	}
	return (0);
}
