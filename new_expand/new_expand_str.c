/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_expand_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:29:24 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 12:59:02 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_str(char *str)
{
	size_t	iter;

	iter = 0;
	while (str[iter])
	{
	}
}

int	new_expand_str(t_input *data)
{
	size_t iter;

	iter = 0;
	while (data->entries[iter])
	{
		if (!(data->input_spec[iter] >= HERE_DOC_OP
				&& data->input_spec[iter] <= OUTFILE_OP))
		{
			data->entries[iter] = expand(data->entries[iter], data);
			if (!data->entries[iter])
				return (-1);
		}
		iter++;
	}
}