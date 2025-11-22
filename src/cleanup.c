#include "philo.h"

static void	destroy_forks(pthread_mutex_t *forks, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->n_philos)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

static void	destroy_rules_mutexes(t_rules *rules)
{
	pthread_mutex_destroy(&rules->write_mutex);
	pthread_mutex_destroy(&rules->dead_mutex);
	pthread_mutex_destroy(&rules->meal_mutex);
}

void	cleanup_all(t_philo *philos, pthread_mutex_t *forks, t_rules *rules)
{
	if (forks)
	{
		destroy_forks(forks, rules);
		free(forks);
	}
	if (philos)
		free(philos);
	destroy_rules_mutexes(rules);
}
