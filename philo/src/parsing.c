#include "philo.h"

static int is_number(const char *str)
{
    int i = 0;

    if (!str || !str[0])
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

static int ft_atoi_positive(const char *str)
{
    long num = 0;

    while (*str)
    {
        num = num * 10 + (*str - '0');
        if (num > INT_MAX)
            return (-1);
        str++;
    }
    return ((int)num);
}

int parse_args(int argc, char **argv, t_args *config)
{
    if (argc != 5 && argc != 6)
    {
        write(2, "Error: wrong number of arguments\n", 33);
        return (1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (!is_number(argv[i]))
        {
            write(2, "Error: arguments must be positive numbers\n", 42);
            return (1);
        }
    }
    config->nbr_philos = ft_atoi_positive(argv[1]);
    config->time_to_die = ft_atoi_positive(argv[2]);
    config->time_to_eat = ft_atoi_positive(argv[3]);
    config->time_to_sleep = ft_atoi_positive(argv[4]);
    config->must_eat_count = (argc == 6) ? ft_atoi_positive(argv[5]) : -1;

    if (config->nbr_philos <= 0 || config->time_to_die <= 0
        || config->time_to_eat <= 0 || config->time_to_sleep <= 0
        || (argc == 6 && config->must_eat_count <= 0))
    {
        write(2, "Error: invalid argument values\n", 31);
        return (1);
    }
    return (0);
}
