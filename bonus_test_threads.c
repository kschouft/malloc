#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "includes/malloc_bonus.h"

#define THREAD_COUNT 4
#define ITERATIONS   1000

void *worker(void *arg)
{
    char *ptr;
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++)
    {
        ptr = malloc_bonus(64, false);
        if (!ptr)
            return ((void *)1);
        memset(ptr, 'a', 64);
        free(ptr);
    }
    return (NULL);
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    void      *ret;
    ft_putstr("-----THREAD TEST-----\n");
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0)
        {
            ft_putstr("ERROR: pthread_create\n");
            return (1);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_join(threads[i], &ret) != 0)
        {
            ft_putstr("ERROR: pthread_join\n");
            return (1);
        }
        if (ret != NULL)
        {
            ft_putstr("ERROR: allocation failed\n");
            return (1);
        }
    }
    ft_putstr("-----THREAD TEST PASSED-----\n");
    return (0);
}