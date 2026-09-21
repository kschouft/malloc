#pragma once
#include <stdint.h>
#include <unistd.h>

void    ft_putchar(const char c);
void    ft_putstr(const char *str);
void    ft_putnbr(size_t nb);
size_t  ft_strlen(const char *str);
void    ft_putnbr_base(long int nb, const char *base, size_t base_size);
void    ft_print_hex(void *ptr);
void    ft_print_hex_dump(unsigned char c);
int     ft_strcmp(char *s1, char *s2);