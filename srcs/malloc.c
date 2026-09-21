#include "../includes/malloc.h"

t_types types = {0};

void    *zone_alloc(size_t size, t_type type)
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
        (*pages) = init_zone(size, page_size);
        if (!*pages)
            return (NULL);
        (*pages)->prev = NULL;
        (*pages)->next = NULL;
        return ((void *)((char *)(*pages) + sizeof(t_page) + sizeof(t_block)));
    }
    else if (type == LARGE)
        return (add_page(size, page_size, type));
    else
        return (find_free_block(size, page_size, type));
}

t_type get_page_type(size_t size)
{
    size += sizeof(t_block);
    if (size <= (TINY_SIZE - sizeof(t_page)) / 100)
        return TINY;
    else if (size <= (SMALL_SIZE - sizeof(t_page)) / 100)
        return SMALL;
    else
        return LARGE;
}


void    *malloc(size_t size)
{
    size_t req_size = size;
    if (size == 0)
        return NULL;
    if (size > INT64_MAX - 15)
        return NULL;
    size = align16(size);
    t_type type = get_page_type(size);
    void* ret = zone_alloc(size, type);
    if (!ret)
        return (NULL);
    t_block* block = (t_block*)((char*)ret - sizeof(t_block));
    block->req_size = req_size;
    return ret;
}