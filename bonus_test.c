#include "includes/malloc_bonus.h"
#include <string.h>

void show_blocks_size()
{
    ft_putstr("MAX TINY SIZE = ");
    ft_putnbr(((TINY_SIZE - sizeof(t_page) - (sizeof(t_block) * 100)) / 100) & ~ 15);
    ft_putchar('\n');
    ft_putstr("MAX SMALL SIZE = ");
    ft_putnbr(((SMALL_SIZE - sizeof(t_page) - (sizeof(t_block) * 100)) / 100) & ~ 15);
    ft_putchar('\n');    
}

int main()
{
    // show_blocks_size();
    // // char* ptr = malloc(16);
    // // strcpy(ptr, "Hello World!");
    // // ft_putstr(ptr);
    // // ft_putchar('\n');
    // char* ptr1[103];
    // for (int i = 0; i < 103; i++)
    // {
    //     ptr1[i] = malloc(112);
    //     if (!ptr1[i])
    //     {
    //         printf("ERROR: malloc failed at block %d\n", i);
    //         return (1);
    //     }
    // }
    // show_alloc_mem();
    // for (int i = 0; i < 103; i++)
    // {
    //     free(ptr1[i]);
    // }
    // ptr1[0] = realloc(ptr1[0], 250);
    // ft_putstr("\n\n\n");
    // char* ptr2 = malloc(20000);
    // show_alloc_mem();
    // ft_putstr("\n\n\n");
    // char* ptr3 = malloc(8);
    // show_alloc_mem();
    // ft_putstr("\n\n\n");
    // ptr3 = realloc(ptr3, 20000);
    // show_alloc_mem();
    // char* ptr4 = malloc(24);
    // free(ptr2);
    // free(ptr3);
    char* ptr4 = malloc(24);
    ptr4 = strcpy(ptr4, "Hello World!");
    show_alloc_mem();
    ft_putchar('\n');
    show_alloc_mem_ex();
}