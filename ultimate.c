/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultimate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:22:37 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 13:34:15 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// typedef struct s_entry
// {
// 	char			*raw_entry;
// 	int				spec;
// 	char			**expanded;
// 	int				exp_count;
// 	struct s_entry	*next;
// }					t_entry;

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

int	is_token(char c)
{
	if (c == 32 || (c >= 9 && c <= 13) || c == '>' || c == '<' || c == '|')
		return (0);
	return (1);
}

int	is_whitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

// void	toggle_quotes(char *str, t_input *data, size_t iter)
// {
// 	if (str[iter] == '\'' && !data->dbl_quote && (data->sgl_quote
// 		|| (ft_strchr(&str[iter + 1], '\''))))
// 		data->sgl_quote = !data->sgl_quote;
// 	else if (str[iter] == '\"' && !data->sgl_quote && (data->dbl_quote
// 			|| (ft_strchr(&str[iter + 1], '\"'))))
// 		data->dbl_quote = !data->dbl_quote;
// }

int	operator_case(char *buf, t_input *data, size_t iter)
{
	if (buf[iter] == '|')
		return (1);
	if (buf[iter] == '>' && buf[iter + 1] != '>')
		return (1);
	if (buf[iter] == '<' && buf[iter + 1] != '<')
		return (1);
	if (buf[iter] == '>' && buf[iter + 1] == '>')
		return (2);
	if (buf[iter] == '<' && buf[iter + 1] == '<')
		return (2);
	return (0);
}

int	token_len(char *buf, t_input *data, size_t iter)
{
	size_t	count;

	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	count = operator_case(buf, data, iter);
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

int	create_list(char *buf, t_input *data)
{
	size_t	iter;
	size_t	entry_len;
	t_entry	*entry;
	char	*raw_str;

	data->entries = NULL;
	iter = 0;
	while (buf[iter])
	{
		entry_len = token_len(buf, data, iter);
		if (!entry_len)
		{
			iter++;
			continue ;
		}
		raw_str = malloc((entry_len + 1) * sizeof(char));
		if (!raw_str)
			return (perror("create_list"), -1);
		ft_strlcpy(raw_str, &buf[iter], entry_len + 1);
		entry = newnode(raw_str);
		if (data->entries == NULL)
			data->entries = entry;
		else
			lstadd(&data->entries, entry);
		iter += entry_len;
	}
	return (0);
}

void	entry_spec(t_input *data)
{
	t_entry	*cur;

	cur = data->entries;
	while (cur)
	{
		if (!ft_strcmp(cur->raw_entry, "<<"))
		{
			cur->spec = HERE_DOC_OP;
			if (cur->next)
				cur->next->spec = HERE_DOC;
		}
		else if (!ft_strcmp(cur->raw_entry, ">>"))
		{
			cur->spec = APPEND_OP;
			if (cur->next)
				cur->next->spec = APPEND_FILE;
		}
		else if (!ft_strcmp(cur->raw_entry, "<"))
		{
			cur->spec = INFILE_OP;
			if (cur->next)
				cur->next->spec = INFILE;
		}
		else if (!ft_strcmp(cur->raw_entry, ">"))
		{
			cur->spec = OUTFILE_OP;
			if (cur->next)
				cur->next->spec = OUTFILE;
		}
		else if (!ft_strcmp(cur->raw_entry, "|"))
			cur->spec = PIPE;
		cur = cur->next;
	}
}

size_t	exp_len(char *env)
{
	size_t	len;

	len = 0;
	if (env[len] == '?')
		return (1);
	if (ft_isalpha(env[len]) || env[len] == '_')
		while (ft_isalnum(env[len]) || env[len] == '_')
			len++;
	return (len);
}

int	expand_count(t_entry *entry, t_input *data)
{
	size_t	iter;
	int		count;

	iter = 0;
	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (entry->raw_entry[iter])
	{
		toggle_quotes(entry->raw_entry, data, iter);
		if (!data->dbl_quote && !data->sgl_quote
			&& entry->raw_entry[iter] == '$' && exp_len(&entry->raw_entry[iter
				+ 1]))
			count++;
	}
	return (count);
}

/*
in expand entry möglicherweise eine list mit
	{
		int		exp_name_len;
		char 	*exp_value;
		int		exp_value_len;
		int		exp_position;
					//bringt wahrscheins nichts weil sich dann expanded string eh verschiebt?
	}						//vielleicht len vom letzten expand damit man den zwischenspace immer skippen kann aber maybe trippin

- alle expands im ersten run speichern und dann nur noch strlen gesamt
	- alle exp_name + alle exp_value

ZIEL	expand_entry(void):
	für entry->expanded = char **expanded strings für ein entry, das bedeutet:
	hi="you     are"
	sh> ls$hi "" -> wird zu [0][lsyou] [1][are] [2][] [3][NULL]

TOTAL GOAL:
	wir gehen alle list entries durch und führen einfach nur die expanded **strings aus.

(Grund für the way it is: einmal tokenized verändert sich meaning nicht egal wie oft expand bei INFILE instant ambiguous redirect bei mehreren wörtern,
	heredoc muss gar nicht expanded werden, wörter kann es eh viele geben.
	Also speichern wir den rawstr für error und heredoc,
		das expanded array für normale commands oder brackets etc
	und nummer damit wir 1 expand bei leerem str sehen und wissen dass es ein entry sein soll und es ein expand gab)
*/
// int	expand_entry(t_entry *entry, t_input *data)
// {
// 	int	*counts;

// 	char *exp_str entry->exp_count = expand_count(entry, data);
// 	exp_str = expand(entry);
// }

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

int	expanded_split(char *exp_str, t_entry *entry, t_input *data)
{
	t_list	*node;
	t_list	*head;
	size_t	iter;
	size_t	entry_len;

	iter = 0;
	head = NULL;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	if (!exp_str[0])
		return (0);
	while (exp_str[iter])
	{
		while (is_whitespace(exp_str[iter]))
			iter++;
		entry_len = token_len(exp_str, data, iter);
		node = ft_lstnew(fill_content(exp_str, iter, data, entry_len));
		if (!head)
			head = node;
		else
			ft_lstadd_back(&head, node);
		iter += entry_len + (entry_len == 0);
	}
	entry->expanded = lst_to_expand(head);
	if (!entry->expanded)
		return (-1);
	return (0);
}

int	expansion(t_input *data)
{
	t_entry	*cur;
	char	*expanded_str;
	size_t	test;

	cur = data->entries;
	while (cur)
	{
		if (!(cur->spec >= HERE_DOC_OP && cur->spec <= OUTFILE_OP)
			&& cur->spec != PIPE && cur->spec != HERE_DOC)
		{
			expanded_str = expand(cur, data);
			if (!expanded_str)
				return (-1);
			if (expanded_split(expanded_str, cur, data) == -1)
				return (-1);
			// test = 0;
			// printf("ENTRY: \n<\n");
			// printf("pointer: %p\n", cur->expanded);
			// printf("exp_count: [%d]\n", cur->exp_count);
			// if (cur->expanded)
			// {
			// 	while (cur->expanded[test])
			// 	{
			// 		printf("content: [%s]\n", cur->expanded[test]);
			// 		test++;
			// 	}
			// 	fflush(stdout);
			// }
			// printf(">\n");
		}
		else if (cur->spec == HERE_DOC_OP)
		{
			if (here_doc(data) == -1)
			{
				if (g_current_signal != SIGINT)
					free2d(&data->envp);
				return (-1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int	ultimate(char *buf, t_input *data)
{
	// create list with raw elements
	if (create_list(buf, data) == -1)
		return (-1);
	// specify operators, files, DEFAULTS
	entry_spec(data);
	// expand each raw_entry inside list_node
	if (expansion(data) == -1)
		return (-1);
	return (0);
}