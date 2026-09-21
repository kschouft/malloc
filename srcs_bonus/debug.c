/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:41:17 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:41:19 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc_bonus.h"

char    debug_check()
{
    char *var = NULL;
    char   env_flag = 0;
    var = getenv("MALLOC_DEBUG");
    if (var && (ft_strcmp(var, "1") == 0))
        env_flag |= 1 << 0;
    var = getenv("MALLOC_LOG");
    if (var && (ft_strcmp(var, "1") == 0))
        env_flag |= 1 << 1;
    return (env_flag);
}
