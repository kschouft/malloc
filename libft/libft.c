/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:40:56 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:40:57 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void    ft_putchar(const char c)
{
    write(1, &c, 1);
}

void    ft_putstr(const char *str)
{
    for (int i = 0; str[i]; ++i)
        write(1, &str[i], 1);
}

void    ft_putnbr(size_t nb)
{
    if (nb >= 10)
        ft_putnbr(nb / 10);
    ft_putchar(nb % 10 + '0');
}

size_t  ft_strlen(const char *str)
{
    int i = 0;
    for ( ; str[i]; ++i)
        ;
    return (i);
}

void    ft_putnbr_base(long int nb, const char *base, size_t base_size)
{
    if (nb <= 0)
        return;
    if (nb >= (long int)base_size)
        ft_putnbr_base((nb / base_size), base, base_size);
    ft_putchar(base[nb % base_size]);
}

void    ft_print_hex(void *ptr)
{
    ft_putstr("0X");
    ft_putnbr_base((long)ptr, "0123456789ABCDEF", 16);
}

void    ft_print_hex_dump(unsigned char c)
{
    const char *base = "0123456789ABCDEF";

    ft_putchar(base[c / 16]);
    ft_putchar(base[c % 16]);
}

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}