/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:52:39 by eprottun          #+#    #+#             */
/*   Updated: 2025/05/09 12:37:34 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(int n)
{
	int	number;

	number = !n;
	if (n < 0)
		number++;
	while (n)
	{
		n /= 10;
		number++;
	}
	return (number);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	number;

	number = n;
	len = digit_count(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	number *= 10;
	if (number < 0)
	{
		str[0] = '-';
		number *= -1;
	}
	if (number == 0)
		str[--len] = '0';
	while (number / 10)
		str[--len] = '0' + ((number /= 10) % 10);
	return (str);
}

// #include <stdio.h>

// int	main(void)
// {
// 	printf("%s\n", ft_itoa(-2147483648));
// 	return (0);
// }