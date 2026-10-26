/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 10:40:50 by keschouf          #+#    #+#             */
/*   Updated: 2026/09/21 10:40:51 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include "libft.h"
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#define TINY_SIZE (PAGE_SIZE * 4)
#define SMALL_SIZE (TINY_SIZE * 16)
#define DEBUG_FLAG (1 << 0)
#define LOG_FLAG   (1 << 1)

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

extern pthread_mutex_t  mutex;
extern  t_types         types;

/*free*/
void    free_bonus(void *ptr, bool flag);
void    free(void *ptr);

/*blocks*/
void    *return_block(t_block* block, size_t size, char env_flag);
void    *find_free_block(size_t size, size_t page_size, t_type type, char env_flag);
t_block *find_block_from_ptr(void *ptr);

/*pages*/
void    *add_page(size_t size, size_t page_size, t_type type, char env_flag);
void    *init_zone(size_t size, size_t page_size, char env_flag);

/*utils*/
size_t  align16(size_t size);
size_t	align_page(size_t size);
size_t  get_min_size(t_type type);
void    show_alloc_mem();
void    show_alloc_mem_ex();

/*malloc*/
t_type  get_page_type(size_t size);
void    *malloc_bonus(size_t size, bool flag);
void    *malloc(size_t size);

/*realloc*/
void    *ft_realloc(void *ptr, size_t size);

/*debug*/
char    debug_check();

/*print*/
void    print_debug_size_req(size_t size);
void    print_allignated_size(size_t size);
void    print_debug_addr(void* ret);
void    print_malloc(size_t size, void* ret);
void    print_page_type_debug(t_type type);