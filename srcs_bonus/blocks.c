/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blocks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:41:16 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:41:17 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc_bonus.h"

void    *return_block(t_block* block, size_t size, char env_flag)
{
    if (block->size >= size && block->size - size >= sizeof(t_block) + 16)
    {
        t_block* tmp = block->next;
        block->next = (t_block *)((char *)block + size + sizeof(t_block));
        block->next->size = block->size - size - sizeof(t_block);
        block->next->free = true;
        block->next->next = tmp;
        block->next->prev = block;
        block->size = size;
        if (tmp)
            tmp->prev = block->next;
    }
    block->free = false;
    if (env_flag & DEBUG_FLAG)
    {
        ft_putstr("[DEBUG] block found\n[DEBUG] block addr = ");
        ft_print_hex((char *)block);
        ft_putstr("\n");
    }
    return (void *)((char *)block + sizeof(t_block));
}

void    *find_free_block(size_t size, size_t page_size, t_type type, char env_flag)
{
    t_page* page;
    if (type == TINY)
        page = types.tinys;
    else if (type == SMALL)
        page = types.smalls;
    else if (type == LARGE)
        page = types.larges;
    while (page != NULL)
    {
        t_block *block = page->blocks;
        while (block != NULL)
        {
            if (block->free && block->size >= size)
                return return_block(block, size, env_flag);
            block = block->next;
        }
        page = page->next;
    }
    return (add_page(size, page_size, type, env_flag));
}

t_block *find_block_from_ptr(void *ptr)
{
    t_page  *page;
    t_block *block;
    if (!ptr)
        return (NULL);
    page = types.tinys;
    while (page)
    {
        block = page->blocks;
        while (block)
        {
            if ((void *)((char *)block + sizeof(t_block)) == ptr)
                return (block);
            block = block->next;
        }
        page = page->next;
    }
    page = types.smalls;
    while (page)
    {
        block = page->blocks;
        while (block)
        {
            if ((void *)((char *)block + sizeof(t_block)) == ptr)
                return (block);
            block = block->next;
        }
        page = page->next;
    }
    page = types.larges;
    while (page)
    {
        block = page->blocks;
        while (block)
        {
            if ((void *)((char *)block + sizeof(t_block)) == ptr)
                return (block);
            block = block->next;
        }
        page = page->next;
    }
    return (NULL);
}
