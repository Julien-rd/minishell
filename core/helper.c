/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:15:30 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 13:11:20 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free2d(char ***str)
{
	size_t	iter;

	iter = 0;
	if (*str)
	{
		while ((*str)[iter])
		{
			free((*str)[iter]);
			iter++;
		}
		free(*str);
	}
	*str = NULL;
}

int	safe_write(int fd, char *buf, size_t len)
{
	if (write(fd, buf, len) == -1)
		return (perror("write"), -1);
	return (0);
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

int	toggle_quotes(char *buf, t_sh *sh, size_t iter)
{
	int	r_value;

	r_value = 0;
	if (buf[iter] == '\'' && !sh->dbl_quote && (sh->sgl_quote
		|| (ft_strchr(&buf[iter + 1], '\''))) && ++r_value)
		sh->sgl_quote = !sh->sgl_quote;
	else if (buf[iter] == '\"' && !sh->sgl_quote && (sh->dbl_quote
			|| (ft_strchr(&buf[iter + 1], '\"'))) && ++r_value)
		sh->dbl_quote = !sh->dbl_quote;
	return (r_value);
}

long long	ft_atoll(const char *str)
{
	long long			i;
	int					sign;
	unsigned long long	tmp_num;

	i = 0;
	sign = 1;
	tmp_num = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	if (str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (tmp_num > (ULLONG_MAX - str[i] - '0') / 10)
			return (0);
		tmp_num = tmp_num * 10 + str[i++] - '0';
	}
	if ((sign == 1 && (tmp_num > LLONG_MAX)) || str[i])
		return (0);
	else if (tmp_num > (unsigned long long)LLONG_MAX + 1)
		return (0);
	return (sign * (long long)tmp_num);
}

char	*remove_quotes(char *to_strip, size_t len)
{
	size_t	iter;
	size_t	stripped_iter;
	t_sh	sh;
	char	*stripped;

	stripped_iter = 0;
	sh.dbl_quote = 0;
	sh.sgl_quote = 0;
	stripped = malloc((len + 1) * sizeof(char));
	if (!stripped)
		return (NULL);
	iter = 0;
	while (iter < len)
	{
		iter += toggle_quotes(to_strip, &sh, iter);
		if (!(to_strip[iter] == '\'' && sh.sgl_quote)
					&& !(to_strip[iter] == '\"' && sh.dbl_quote))
			stripped[stripped_iter++] = to_strip[iter];
		toggle_quotes(to_strip, &sh, iter);
		iter++;
	}
	stripped[stripped_iter] = '\0';
	return (stripped);
}

size_t	skip_whitspaces(char *buf)
{
	size_t	iter;

	iter = 0;
	while ((buf[iter] >= 9 && buf[iter] <= 13) || buf[iter] == 32)
		iter++;
	return (iter);
}

size_t	empty_prompt(char *buf)
{
	return (!ft_strlen(buf));
}

void	cut_nl(char *buf)
{
	size_t	len;

	len = ft_strlen(buf);
	if (len == 0)
		return ;
	buf[len - 1] = '\0';
}

void	free_list(t_entry *list)
{
	t_entry	*tmp;

	while (list)
	{
		if (list->raw_entry)
		{
			free(list->raw_entry);
			list->raw_entry = NULL;
		}
		if (list->expanded)
		{
			free2d(&list->expanded);
			list->expanded = NULL;
		}
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

void	child_exit_handle(t_sh *sh, t_pipeline *pl, int errcode)
{
	free2d(&sh->envp.vars);
	free_list(sh->entries);
	// free(sh->pipe.position);
	// free(cmd->cmd);
	if (sh->heredoc)
		free2d(&sh->heredoc);
	exit(errcode);
}
