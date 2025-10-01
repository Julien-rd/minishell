/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:17:28 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 17:20:25 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_entry	*lstlast(t_entry *lst)
{
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	lstadd(t_entry **lst, t_entry *new)
{
	t_entry	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	
	last = lstlast(*lst);
	last->next = new;
}

t_entry	*newnode(char *raw_str)
{
	t_entry	*new_lst;

	new_lst = malloc(sizeof(t_entry));
	if (new_lst == NULL)
		return (NULL);
	new_lst->raw_entry = raw_str;
	new_lst->expanded = NULL;
	new_lst->exp_count = 0;
	new_lst->spec = DEFAULT;
	new_lst->next = NULL;
	return (new_lst);
}