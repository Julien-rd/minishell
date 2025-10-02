/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:14:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 10:40:54 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check(t_input *data)
{
	size_t	iter;
	t_entry	*entry;

	entry = data->entries;
	iter = 0;
	while (entry != NULL)
	{
		if (entry->spec == PIPE)
		{
			if (entry->next == NULL || entry->next->spec == PIPE
				|| entry == data->entries)
			{
				write(1, "syntax error near unexpected token `|'\n", 40);
				return (-1);
			}
		}
		if (entry->spec == INFILE_OP && (entry->next == NULL
				|| entry->next->spec != INFILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == HERE_DOC_OP && (entry->next == NULL
				|| entry->next->spec != HERE_DOC))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == OUTFILE_OP && (entry->next == NULL
				|| entry->next->spec != OUTFILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == APPEND_OP && (entry->next == NULL
				|| entry->next->spec != APPEND_FILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		entry = entry->next;
	}
	return (0);
}
