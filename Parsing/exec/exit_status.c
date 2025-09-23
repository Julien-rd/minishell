/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:18:46 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 15:52:40 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long long	exit_status(const char *str)
{
	long long			i;
	int					sign;
	unsigned long long	return_number;

	i = 0;
	sign = 1;
	return_number = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (return_number > (LLONG_MAX - (str[i] - 48)) / 10)
			return (-1);
		return_number *= 10;
		return_number += str[i] - 48;
		i++;
	}
	return_number *= sign;
	// while (return_number >= 255)
	// 	return_number = return_number / 256 - 1;
	return (return_number);
}
