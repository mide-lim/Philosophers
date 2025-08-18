#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_config
{
    int philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int must_eat_count; // opcional
}   t_config;

// parsing.c
int     parse_arguments(int argc, char **argv, t_config *config);

#endif
