#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>
#include "libft.h"
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#define TINY_SIZE (PAGE_SIZE * 4)
#define SMALL_SIZE (TINY_SIZE * 16)

typedef enum e_type
{
    TINY = 1,
    SMALL,
    LARGE,
}           t_type;

typedef struct s_page
{
    size_t        size;
    struct  s_page  *next;
    struct  s_page  *prev;
    struct  s_block *blocks;   
}             t_page;

typedef struct  s_block
{
    size_t          size;
    bool            free;
    size_t          req_size;
    struct s_block  *next;
    struct s_block  *prev;
    char            padding[8];
}               t_block;

typedef struct s_types
{
    t_page  *tinys;
    t_page  *smalls;
    t_page  *larges;
}               t_types;

extern  t_types    types;

/*free*/
void    free(void *ptr);

/*blocks*/
void    *return_block(t_block* block, size_t size);
void    *find_free_block(size_t size, size_t page_size, t_type type);
t_block *find_block_from_ptr(void *ptr);

/*pages*/
void    *add_page(size_t size, size_t page_size, t_type type);
void    *init_zone(size_t size, size_t page_size);

/*utils*/
size_t  align16(size_t size);
size_t	align_page(size_t size);
void    show_alloc_mem(void);
size_t  get_min_size(t_type type);

/*malloc*/
t_type  get_page_type(size_t size);
void    *malloc(size_t size);

/*realloc*/
void    *realloc(void *ptr, size_t size);
