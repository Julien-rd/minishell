/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:44:25 by eprottun          #+#    #+#             */
/*   Updated: 2025/04/26 17:47:28 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	last = ft_lstlast(*lst);
	if (!last)
	{
		(*lst) = new;
		return ;
	}
	last->next = new;
}
/*
#include <stdio.h>

int	main()
{
	t_list	*first;
	t_list	*second;
	t_list	*third;
	
	first = malloc(sizeof(t_list));
    second = malloc(sizeof(t_list));
    third = malloc(sizeof(t_list));
	if (!first || !second || !third)
        return (1);
	first->content = (void *)1;
	first->next = second;
	second->content = (void *)2;
	second->next = NULL;
	third->content = (void *)3;
	third->next = NULL;
	ft_lstadd_back(&first, third);
	printf("%d\n", (int)(first->next)->content);
	printf("%d\n", (int)(second->next)->content);
	return (0);
}*/
