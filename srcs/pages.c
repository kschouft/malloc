/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:41:10 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:41:11 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void    *add_page(size_t size, size_t page_size, t_type type)
{
    t_page *page;
    t_page *new_page;
    if (type == TINY)
        page = types.tinys;
    else if (type == SMALL)
        page = types.smalls;
    else
        page = types.larges;
    while (page->next != NULL)
        page = page->next;
    new_page = init_zone(size, page_size);
    if (!new_page)
        return (NULL);
    page->next = new_page;
    new_page->prev = page;
    new_page->next = NULL;
    return ((void *)((char *)new_page + sizeof(t_page) + sizeof(t_block)));
}

void *init_zone(size_t size, size_t page_size)
{
    t_page  *page;
    t_block *block;
    t_block *free_block;
    size_t  remaining;
    page = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANON, -1, 0);
    if (page == MAP_FAILED)
        return (NULL);
    page->size = page_size;
    page->blocks = (t_block *)((char *)page + sizeof(t_page));
    block = page->blocks;
    block->prev = NULL;
    block->size = size;
    block->free = false;
    remaining = page_size - sizeof(t_page) - sizeof(t_block) - size;
    if (remaining >= sizeof(t_block) + 16)
    {
        free_block = (t_block *)((char *)block + sizeof(t_block) + size);
        free_block->prev = block;
        free_block->size = remaining - sizeof(t_block);
        free_block->free = true;
        free_block->next = NULL;
        block->next = free_block;
    }
    else
    {
        block->next = NULL;
        block->size += remaining;
    }
    return (page);
}