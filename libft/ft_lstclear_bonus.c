/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:11:43 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/26 17:56:54 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp_next;

	while (*lst)
	{
		temp_next = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = temp_next;
	}
	*lst = NULL;
}
