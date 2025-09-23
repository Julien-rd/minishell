/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:57:02 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 18:18:46 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free2d(char **str)
{
    size_t iter;

    iter = 0;
    if (str)
    {
        while(str[iter])
        {
            free(str[iter]);
            iter++;
        }
        free(str);
    }
    str = NULL;
}