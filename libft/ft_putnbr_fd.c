/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:48:38 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/25 14:03:42 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	print[12];
	int		i;
	size_t	j;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	i = 11;
	j = 0;
	if (n == 0)
		write(fd, "0", 1);
	if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
	}
	while (n != 0 && i >= 0)
	{
		print[i--] = '0' + (n % 10);
		n /= 10;
		j++;
	}
	write(fd, &print[12 - j], j);
}
