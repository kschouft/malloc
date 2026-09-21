#include "../includes/malloc_bonus.h"

char    debug_check()
{
    char *var = NULL;
    char   env_flag = 0;
    var = getenv("MALLOC_DEBUG");
    if (var && (ft_strcmp(var, "1") == 0))
        env_flag |= 1 << 0;
    var = getenv("MALLOC_LOG");
    if (var && (ft_strcmp(var, "1") == 0))
        env_flag |= 1 << 1;
    return (env_flag);
}
