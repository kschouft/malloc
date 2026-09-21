#include "../includes/malloc_bonus.h"

void    print_debug_size_req(size_t size)
{
    ft_putstr("[DEBUG] malloc requested\n[DEBUG] requested size = ");
    ft_putnbr(size);
    ft_putstr("\n");   
}

void    print_allignated_size(size_t size)
{
    ft_putstr("[DEBUG] aligned size = ");
    ft_putnbr(size);
    ft_putstr("\n");
}

void    print_debug_addr(void* ret)
{
    ft_putstr("[DEBUG] malloc addr = ");
    ft_print_hex(ret);
    ft_putstr("\n");
}

void    print_malloc(size_t size, void* ret)
{
    ft_putstr("[MALLOC] malloc(");
    ft_putnbr(size);
    ft_putstr(") -> ");
    ft_print_hex(ret);
    ft_putstr("\n");
}

void    print_page_type_debug(t_type type)
{
    if (type == 1)
        ft_putstr("[DEBUG] page type = TINY\n");
    else if (type == 2)
        ft_putstr("[DEBUG] page type = SMALL\n");
    else
        ft_putstr("[DEBUG] page type = LARGE\n");
}