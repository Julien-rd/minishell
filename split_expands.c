/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:13:37 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 20:05:57 by eprottun         ###   ########.fr       */
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

int	token_len(char *buf, t_input *data, size_t iter)
{
	size_t	count;

	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	count = operator_case(buf, iter);
	if (count > 0)
		return (count);
	while (data->dbl_quote || data->sgl_quote || (buf[iter]
			&& is_token(buf[iter])))
	{
		toggle_quotes(buf, data, iter);
		count++;
		iter++;
	}
	return (count);
}

char	*fill_content(char *exp_str, size_t iter, t_input *data,
		size_t entry_len)
{
	size_t	entry_iter;
	char	*content;

	entry_iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	content = malloc((entry_len + 1) * sizeof(char));
	if (!content)
		return (NULL);
	while (exp_str[iter]
		&& !is_whitespace(exp_str[iter]) | data->dbl_quote | data->sgl_quote)
	{
		iter += toggle_quotes(exp_str, data, iter);
		if (!((exp_str[iter] == '\'' && data->sgl_quote)
					|| (exp_str[iter] == '\"' && data->dbl_quote)))
			content[entry_iter++] = exp_str[iter];
		iter++;
	}
	content[entry_iter] = '\0';
	return (content);
}

char	**lst_to_expand(t_list *head)
{
	char	**expanded;
	size_t	iter;

	iter = 0;
	expanded = malloc((ft_lstsize(head) + 1) * sizeof(char *));
	if (!expanded)
		return (NULL);
	while (head)
	{
		expanded[iter] = head->content;
		iter++;
		head = head->next;
	}
	expanded[iter] = NULL;
	return (expanded);
}

int	split_expands(char *exp_str, t_entry *entry, t_input *data)
{
	t_list	*node;
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
		entry_len = token_len(exp_str, data, iter);
		node = ft_lstnew(fill_content(exp_str, iter, data, entry_len));
		ft_lstadd_back(&head, node);
		iter += entry_len + (entry_len == 0);
	}
	entry->expanded = lst_to_expand(head);
	if (!entry->expanded)
		return (-1);
	return (0);
}
