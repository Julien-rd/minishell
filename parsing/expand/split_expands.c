/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:13:37 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:52:08 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	operator_case(char *buf, size_t iter)
{
	if (!ft_strncmp(&buf[iter], ">>", 2)
		|| !ft_strncmp(&buf[iter], "<<", 2))
		return (2);
	if (buf[iter] == '>' || buf[iter] == '<' || buf[iter] == '|')
		return (1);
	return (0);
}

char	**lst_to_expand(t_list *head)
{
	char	**expanded;
	size_t	iter;

	iter = 0;
	expanded = malloc((ft_lstsize(head) + 1) * sizeof(char *));
	if (!expanded)
		return (perror("lst_to_expand"), NULL);
	while (head)
	{
		expanded[iter] = head->content;
		iter++;
		head = head->next;
	}
	expanded[iter] = NULL;
	return (expanded);
}

int	content_to_lst(t_list	**head, char *exp_str, size_t entry_len)
{
	t_list	*node;
	char	*content;

	content = remove_quotes(exp_str, entry_len);
	if (!content)
		return (perror("content_to_lst"), -1);
	node = ft_lstnew(content);
	if (!node)
		return (perror("content_to_lst"), free(content), -1);
	// free(exp_str);
	ft_lstadd_back(head, node);
	return (0);
}

int	token_len(char *buf, t_sh *sh, size_t iter)
{
	size_t	count;

	count = 0;
	sh->dbl_quote = 0;
	sh->sgl_quote = 0;
	count = operator_case(buf, iter);
	if (count > 0)
		return (count);
	while (sh->dbl_quote || sh->sgl_quote || (buf[iter]
			&& is_token(buf[iter])))
	{
		toggle_quotes(buf, sh, iter);
		count++;
		iter++;
	}
	return (count);
}

int	split_expands(char *exp_str, t_entry *entry, t_sh *sh)
{
	t_list	*head;
	size_t	iter;
	size_t	entry_len;

	iter = 0;
	head = NULL;
	if (!exp_str[0])
		return (0);
	while (exp_str[iter])
	{
		iter += skip_whitspaces(&exp_str[iter]);
		if (!exp_str[iter])
			break;
		entry_len = token_len(exp_str, sh, iter);
		if (content_to_lst(&head, exp_str, entry_len) == -1)
			return (free(exp_str), -1);
		iter += entry_len + (entry_len == 0);
	}
	entry->expanded = lst_to_expand(head);
	if (!entry->expanded)
		return (-1);
	return (0);
}
