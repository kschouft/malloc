#include "../includes/malloc_bonus.h"

void    free_page(void  *ptr, char env_flag)
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
    if(env_flag & DEBUG_FLAG)
    {
        ft_putstr("[DEBUG] unmapping a page\n[DEBUG] page addr = ");
        ft_print_hex(page);
        ft_putstr("\n");
    }
    munmap(page, page->size);
}

void    blocks_merge(void *ptr, char env_flag)
{
    t_block *tmp = (t_block *)((char *)ptr - sizeof(t_block));
    size_t total = tmp->size;
    if (tmp->next != NULL && tmp->next->free)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] merging a block\n");
        tmp->size = (total += tmp->next->size + sizeof(t_block));
        tmp->next = tmp->next->next;
        if (tmp->next != NULL)
            tmp->next->prev = tmp;
    }
    if (tmp->prev != NULL && tmp->prev->free)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] merging a block\n");
        tmp->prev->size += total + sizeof(t_block);
        tmp->prev->next = tmp->next;
        if (tmp->next != NULL)
            tmp->next->prev = tmp->prev;
        tmp = tmp->prev;
    }
    if (tmp->prev == NULL && tmp->next == NULL)
        free_page(tmp, env_flag);
}

void    free_bonus(void *ptr, bool flag)
{
    if (flag == false)
        pthread_mutex_lock(&mutex);
    char env_flag = debug_check();
    if (env_flag & DEBUG_FLAG)
        ft_putstr("[DEBUG] free requested\n");
    if (ptr == NULL)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] free a null ptr requested\n[DEBUG] free failed\n");
        if (flag == false)
            pthread_mutex_unlock(&mutex);
        return;
    }
    t_block* block = find_block_from_ptr(ptr);
    if (!block)
    {
        if (flag == false)
            pthread_mutex_unlock(&mutex);
        return;
    }
    if (block->free)
    {
        if (env_flag & LOG_FLAG)
            ft_putstr("[MALLOC] double free detected\n");
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] double free detected\n");
        ft_putstr("free(): double free detected\n");
        if (flag == false)
            pthread_mutex_unlock(&mutex);
        return;
    }    
    block->free = true;
    blocks_merge(ptr, env_flag);
    if (env_flag & LOG_FLAG)
    {
        ft_putstr("[MALLOC] free(");
        ft_print_hex(ptr);
        ft_putstr(")\n");
    }
    if (flag == false)
        pthread_mutex_unlock(&mutex);
}

void    free(void *ptr)
{
    free_bonus(ptr, false);
}