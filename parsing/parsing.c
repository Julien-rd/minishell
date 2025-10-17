/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:23:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/17 16:18:28 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	here_doc_caller(t_sh *sh)
{
	sh->heredoc = NULL;
	if (here_doc(sh) == -1)
	{
		if (g_current_signal == SIGINT)
			sh->exit_code = 130;
		else
		{
			free2d(&sh->envp.vars);
			sh->exit_code = -1;
		}
		return (-1);
	}
	return (0);
}

static int	expand_raw_entry(t_sh *sh)
{
	t_entry	*cur;
	char	*expanded_str;

	cur = sh->entries;
	while (cur)
	{
		if (!(cur->spec >= HERE_DOC_OP && cur->spec <= OUTFILE_OP)
			&& cur->spec != PIPE && cur->spec != HERE_DOC)
		{
			puts(cur->quotes);
			fflush(stdout);
			expanded_str = expand(cur, sh, DEFAULT);
			if (!expanded_str)
				return (-1);
			puts(expanded_str);
			puts(cur->quotes);
			fflush(stdout);
			if (split_expands(expanded_str, cur, sh) == -1)
				return (free(expanded_str), -1);
			free(expanded_str);
		}
		cur = cur->next;
	}
	return (0);
}

static void	entry_spec(t_sh *sh)
{
	t_entry	*cur;

	cur = sh->entries;
	while (cur)
	{
		if (!ft_strcmp(cur->raw_entry, "<<"))
			cur->spec = HERE_DOC_OP;
		else if (!ft_strcmp(cur->raw_entry, ">>"))
			cur->spec = APPEND_OP;
		else if (!ft_strcmp(cur->raw_entry, "<"))
			cur->spec = INFILE_OP;
		else if (!ft_strcmp(cur->raw_entry, ">"))
			cur->spec = OUTFILE_OP;
		else if (!ft_strcmp(cur->raw_entry, "|"))
			cur->spec = PIPE;
		if (cur->next && cur->spec == HERE_DOC_OP)
			cur->next->spec = HERE_DOC;
		else if (cur->next && cur->spec == APPEND_OP)
			cur->next->spec = APPEND_FILE;
		else if (cur->next && cur->spec == INFILE_OP)
			cur->next->spec = INFILE;
		else if (cur->next && cur->spec == OUTFILE_OP)
			cur->next->spec = OUTFILE;
		cur = cur->next;
	}
}

char	*quote_spec(t_sh *sh, char *raw_str, size_t len)
{
	char *ret_str;
	size_t	iter;
	size_t	ret_iter;

	iter = 0;
	sh->dbl_quote = 0;
	sh->sgl_quote = 0;
	ret_iter = 0;
	ret_str = malloc((len + 1) * sizeof(char));
	if (!ret_str)
		return (perror("quote_spec"), NULL);
	while (raw_str[iter])
	{
		while (toggle_quotes(raw_str, sh, iter))
			iter++;
		if (!raw_str[iter])
			break ;
		if (!(raw_str[iter] == '\'' && sh->sgl_quote)
			&& !(raw_str[iter] == '\"' && sh->dbl_quote))
			ret_str[ret_iter++] = '0' + sh->sgl_quote + sh->dbl_quote * 2;
		iter++;
	}
	ret_str[ret_iter] = '\0';
	return (ret_str);
}

int	init_entry(t_sh *sh, t_entry **entry, char *buf, size_t iter)
{
	size_t	entry_len;
	char	*raw_str;
	char	*unquoted;
	char	*quotes;

	entry_len = token_len(buf, sh, iter);
	raw_str = malloc((entry_len + 1) * sizeof(char));
		if (!raw_str)
			return (perror("init_entry"), -1);
	ft_strlcpy(raw_str, &buf[iter], entry_len + 1);
	unquoted = remove_quotes(&buf[iter], entry_len);
	if (!unquoted)
		return (free(raw_str), -1);
	quotes = quote_spec(sh, raw_str, ft_strlen(unquoted));
	if (!quotes)
		return (free(raw_str), free(unquoted), -1);
	*entry = newnode(raw_str, unquoted, quotes);
	if (!*entry)
		return (free(raw_str), free(unquoted), free(quotes), -1);
	return (0);
}

static int	create_list(char *buf, t_sh *sh)
{
	size_t	iter;
	t_entry	*entry;

	sh->entries = NULL;
	entry = NULL;
	iter = 0;
	while (buf[iter])
	{
		iter += skip_whitspaces(&buf[iter]);
		if (!buf[iter])
			break ;
		if (init_entry(sh, &entry, buf, iter) == -1)
			return (-1);
		lstadd(&sh->entries, entry);
		iter += token_len(buf, sh, iter);
	}
	return (0);
}

int	parsing(char *buf, t_sh *sh)
{
	if (create_list(buf, sh) == -1)
		return (free_list(sh->entries), sh->exit_code = -1, -1);
	entry_spec(sh);
	if (expand_raw_entry(sh) == -1)
		return (free_list(sh->entries), sh->exit_code = -1, -1);
	if (here_doc_caller(sh) == -1)
		return (free_list(sh->entries), -1);
	return (0);
}
