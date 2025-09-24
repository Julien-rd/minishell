/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:50:16 by eprottun          #+#    #+#             */
/*   Updated: 2025/05/03 16:16:44 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_start;
	void	*apply_f;

	new_start = NULL;
	while (lst)
	{
		apply_f = f(lst->content);
		if (!apply_f)
			return (ft_lstclear(&new_start, del), NULL);
		new_node = malloc(sizeof(t_list));
		if (!new_node)
			return (ft_lstclear(&new_start, del), NULL);
		new_node->content = apply_f;
		new_node->next = NULL;
		lst = lst->next;
		if (new_start)
			ft_lstadd_back(&new_start, new_node);
		else
			new_start = new_node;
	}
	return (new_start);
}
