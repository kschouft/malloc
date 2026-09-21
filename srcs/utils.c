#include "../includes/malloc.h"

size_t align16(size_t size)
{
    return ((size + 15) & ~15);
}

size_t	align_page(size_t size)
{
	size_t	page_size;
	page_size = PAGE_SIZE;
	return (((size + page_size - 1) / page_size) * page_size);
}

size_t get_min_size(t_type type)
{
    size_t min_size;

    min_size = 0;
    if (type == TINY)
        min_size = (TINY_SIZE - sizeof(t_page)) / 100;
    else if (type == SMALL)
        min_size = (SMALL_SIZE - sizeof(t_page)) / 100;
    return (min_size);
}

t_page *find_next_page_in_list(t_page *page, t_page *last, t_page *next)
{
    while (page)
    {
        if ((last == NULL || (uintptr_t)page > (uintptr_t)last)
            && (next == NULL || (uintptr_t)page < (uintptr_t)next))
            next = page;
        page = page->next;
    }
    return (next);
}

t_page *get_next_page(t_page *last)
{
    t_page *next;

    next = NULL;
    next = find_next_page_in_list(types.tinys, last, next);
    next = find_next_page_in_list(types.smalls, last, next);
    next = find_next_page_in_list(types.larges, last, next);
    return (next);
}

t_type get_type_from_page(t_page *target)
{
    t_page *page;

    page = types.tinys;
    while (page)
    {
        if (page == target)
            return (TINY);
        page = page->next;
    }
    page = types.smalls;
    while (page)
    {
        if (page == target)
            return (SMALL);
        page = page->next;
    }
    return (LARGE);
}

void print_page_type(t_type type)
{
    if (type == TINY)
        ft_putstr("TINY : ");
    else if (type == SMALL)
        ft_putstr("SMALL : ");
    else
        ft_putstr("LARGE : ");
}

size_t show_page_blocks(t_page *page)
{
    t_block *block;
    size_t  total;

    total = 0;
    block = page->blocks;
    while (block)
    {
        if (!block->free)
        {
            ft_print_hex((char *)block + sizeof(t_block));
            ft_putstr(" - ");
            ft_print_hex((char *)block + sizeof(t_block)
                + block->req_size);
            ft_putstr(" : ");
            ft_putnbr(block->req_size);
            ft_putstr(" bytes\n");
            total += block->req_size;
        }
        block = block->next;
    }
    return (total);
}

void show_alloc_mem(void)
{
    t_page  *page;
    t_page  *last;
    size_t  total;

    total = 0;
    last = NULL;
    page = get_next_page(last);
    while (page)
    {
        print_page_type(get_type_from_page(page));
        ft_print_hex(page);
        ft_putstr("\n");

        total += show_page_blocks(page);

        last = page;
        page = get_next_page(last);
    }
    if (total)
    {
        ft_putstr("Total : ");
        ft_putnbr(total);
        ft_putstr(" bytes\n");
    }
}