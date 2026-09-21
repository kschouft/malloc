/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:41:24 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:41:25 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc_bonus.h"

void    block_cpy(char* ptr, char* new_ptr)
{
    t_block* block = (t_block*)((char*)ptr - sizeof(t_block));
    size_t size = block->req_size;
    for(size_t i = 0; i < size; ++i)
        new_ptr[i] = ptr[i];
}

void*   realloc_merge(t_block *block, size_t size)
{
    if ((block->size + block->next->size > size) && block->next->size >= (size - block->size) + 16)
    {
        t_block* tmp = block->next->next;
        size_t next_size = block->next->size;
        block->next = (t_block *)((char *)block + size + sizeof(t_block));
        block->next->size = next_size - (size - block->size);
        block->next->free = true;
        block->next->next = tmp;
        block->next->prev = block;
        block->size = size;       
        if (tmp)
            tmp->prev = block->next; 
    }
    else
    {
        block->size += block->next->size + sizeof(t_block);
        block->next = block->next->next;
        if (block->next)
            block->next->prev = block;        
    }
    return((char*)block + sizeof(t_block));
}

void split_block(t_block *block, size_t size)
{
    t_block *new_block;
    t_block *old_next;
    size_t old_size;
    old_size = block->size;
    if (old_size - size < sizeof(t_block) + 16)
        return;
    old_next = block->next;
    new_block = (t_block *)((char *)block + sizeof(t_block) + size);
    new_block->size = old_size - size - sizeof(t_block);
    new_block->free = true;
    new_block->prev = block;
    new_block->next = old_next;
    block->size = size;
    block->next = new_block;
    if (old_next)
        old_next->prev = new_block;
    if (old_next && old_next->free)
    {
        new_block->size += sizeof(t_block) + old_next->size;
        new_block->next = old_next->next;
        if (new_block->next)
            new_block->next->prev = new_block;
    }
}

void*   realloc_bonus(void *ptr, size_t size, char env_flag)
{
    (void)env_flag;
    size_t req_size = size;
    if (size > INT64_MAX - 15)
        return NULL;
    if (ptr == NULL)
        return (malloc_bonus(size, true));
    t_block* block = find_block_from_ptr(ptr);
    if (!block || block->free)
        return (NULL);
    if (size == 0)
    {
        free_bonus(ptr, true);
        return (NULL);
    }
    size = align16(size);
    t_type type = get_page_type(block->size);
    if (size == block->size)
    {
        block->req_size = req_size;
        return (ptr);
    }
    if (size < block->size)
    {
        split_block(block, size);
        block->req_size = req_size;
        return (ptr);
    }
    else if (block->next && block->next->free && (block->size + block->next->size + sizeof(t_block) >= size) 
                && size + sizeof(t_block) <= get_min_size(type))
    {
        void *ret = realloc_merge(block, size);
        block->req_size = req_size;
        return (ret);
    }
    void* new_ptr = malloc_bonus(req_size, true);
    if (!new_ptr)
        return (NULL);
    block_cpy((char*)ptr, (char*)new_ptr);
    free_bonus(ptr, true);
    return (new_ptr);
}

void*   realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&mutex);
    char env_flag = debug_check();
    void* ret = realloc_bonus(ptr, size, env_flag);
    if (env_flag & DEBUG_FLAG)
    {
        ft_putstr("[DEBUG] reallocate ");
        ft_print_hex(ptr);
        ft_putstr("\n");
        ft_putstr("[DEBUG] requested reallocate size = ");
        ft_putnbr(size);
        ft_putstr("\n");
        ft_putstr("[DEBUG] reallocate new addr = ");
        ft_print_hex(ret);
        ft_putstr("\n");
    }
    if (env_flag & LOG_FLAG)
    {
        ft_putstr("[MALLOC] reallocate (");
        ft_print_hex(ptr);
        ft_putstr(") -> ");
        ft_print_hex(ret);
        ft_putstr("\n");
    }
    pthread_mutex_unlock(&mutex);
    return (ret);
}