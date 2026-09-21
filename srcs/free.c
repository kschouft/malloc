/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:41:06 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:41:07 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void    free_page(void  *ptr)
{
    t_page *page = (t_page *)((char *)ptr - sizeof(t_page));
    if (page->prev == NULL)
    {
        if (page->next == NULL)
            return ;
        else
        {
            if (page == types.tinys)
                types.tinys = page->next;
            else if (page == types.smalls)
                types.smalls = page->next;
            else if (page == types.larges)
                types.larges = page->next;
            page->next->prev = page->prev;
        }
    }
    else
    {
        if (page->next != NULL)
            page->next->prev = page->prev;
        if (page->prev != NULL)
            page->prev->next = page->next;
    }
    munmap(page, page->size);
}

void    blocks_merge(void *ptr)
{
    t_block *tmp = (t_block *)((char *)ptr - sizeof(t_block));
    size_t total = tmp->size;
    if (tmp->next != NULL && tmp->next->free)
    {
        tmp->size = (total += tmp->next->size + sizeof(t_block));
        tmp->next = tmp->next->next;
        if (tmp->next != NULL)
            tmp->next->prev = tmp;
    }
    if (tmp->prev != NULL && tmp->prev->free)
    {
        tmp->prev->size += total + sizeof(t_block);
        tmp->prev->next = tmp->next;
        if (tmp->next != NULL)
            tmp->next->prev = tmp->prev;
        tmp = tmp->prev;
    }
    if (tmp->prev == NULL && tmp->next == NULL)
        free_page(tmp);
}

void    free(void *ptr)
{
    if (ptr == NULL)
        return;
    t_block* block = find_block_from_ptr(ptr);
    if (!block)
        return;
    if (block->free)
    {
        ft_putstr("double free detected\n");
        return;
    }
    block->free = true;
    blocks_merge(ptr);
}