/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:15:30 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 14:20:21 by jromann          ###   ########.fr       */
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
