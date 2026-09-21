#include "../includes/malloc_bonus.h"

t_types         types = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void    *zone_alloc(size_t size, t_type type, char env_flag)
{
    t_page** pages;
    size_t page_size;
    if (type == TINY)
    {
        pages = &types.tinys;
        page_size = TINY_SIZE;
    }
    else if (type == SMALL)
    {
        pages = &types.smalls;
        page_size = SMALL_SIZE;
    }
    else
    {
        size_t max_page_size;
        pages = &types.larges;
        max_page_size = (size_t)PAGE_SIZE;
        if (size > (size_t)INT64_MAX - sizeof(t_page) - sizeof(t_block))
            return (NULL);
        page_size = size + sizeof(t_page) + sizeof(t_block);
        if (page_size > (size_t)INT64_MAX - (max_page_size - 1))
            return (NULL);
        page_size = align_page(page_size);
    }
    if (*pages == NULL)
    {
        (*pages) = init_zone(size, page_size, env_flag);
        if (!*pages)
            return (NULL);
        if (env_flag & DEBUG_FLAG)
        {
            ft_putstr("[DEBUG] adding a page\n");
            print_page_type_debug(type);
        }
        (*pages)->prev = NULL;
        (*pages)->next = NULL;
        return (void *)((char *)(*pages) + sizeof(t_page) + sizeof(t_block));
    }
    else if (type == LARGE)
    {
        if (env_flag & DEBUG_FLAG)
        {
            ft_putstr("[DEBUG] adding a page\n");
            print_page_type_debug(type);
        }
        return (add_page(size, page_size, type, env_flag));
    }
    else
        return (find_free_block(size, page_size, type, env_flag));
}

void    *malloc_bonus(size_t size, bool flag)
{
    if (flag == false)
        pthread_mutex_lock(&mutex);
    char env_flag = debug_check();
    size_t req_size = size;
    if (env_flag & DEBUG_FLAG)
        print_debug_size_req(req_size);
    if (size == 0)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] null size requested\n[DEBUG] malloc failed\n");
        if (flag == false)
            pthread_mutex_unlock(&mutex);
        return NULL;
    }
    if (size > INT64_MAX - 15)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] invalid size requested\n[DEBUG] malloc failed\n");
        if (flag == false)    
            pthread_mutex_unlock(&mutex);
        return NULL;
    }
    size = align16(size);
    t_type type = get_page_type(size);
    if (env_flag & DEBUG_FLAG)
        print_allignated_size(size);
    void* ret = zone_alloc(size, type, env_flag);
    if (!ret)
    {
        if (env_flag & DEBUG_FLAG)
            ft_putstr("[DEBUG] malloc failed\n");
        if (flag == false)
            pthread_mutex_unlock(&mutex);
        return (NULL);
    }
    if (env_flag & DEBUG_FLAG)
        print_debug_addr(ret);
    if (env_flag & LOG_FLAG)
        print_malloc(req_size, ret);
    t_block* block = (t_block*)((char*)ret - sizeof(t_block));
    block->req_size = req_size;
    if (flag == false)
        pthread_mutex_unlock(&mutex);
    return (ret);    
}

void    *malloc(size_t size)
{
    return(malloc_bonus(size, false));
}