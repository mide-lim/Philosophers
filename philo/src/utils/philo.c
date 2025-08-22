#include "philo.h"

void    *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    printf("Philosopher %d started\n", philo->id);
    pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
    printf("Philosopher %d picked up left fork %d\n", philo->id, philo->left_fork);

    pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
    printf("Philosopher %d picked up right fork %d\n", philo->id, philo->right_fork);

    printf("Philosopher %d is eating 🍝\n", philo->id);
    ft_usleep(philo->data->time_to_eat);

    pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
    printf("Philosopher %d released forks\n", philo->id);

    return (NULL);
}