#include "philo.h"

void    *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    printf("Philosopher %d started\n", philo->id);
    return (NULL);
}