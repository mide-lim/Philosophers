#include "philo.h"

static int	init_forks(pthread_mutex_t **forks, t_rules *rules)
{
	int	i;

	*forks = malloc(sizeof(pthread_mutex_t) * rules->n_philos);
	if (!*forks)
		return (1);
	i = 0;
	while (i < rules->n_philos)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

static void	init_one_philo(t_philo *p, int i, pthread_mutex_t *forks,
		t_rules *rules)
{
	p->id = i + 1;
	p->meals_eaten = 0;
	p->last_meal = 0;
	p->rules = rules;
	p->left_fork = &forks[i];
	p->right_fork = &forks[(i + 1) % rules->n_philos];
}

static int	init_philos(t_philo **philos,
						pthread_mutex_t *forks,
						t_rules *rules)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * rules->n_philos);
	if (!*philos)
		return (1);
	i = 0;
	while (i < rules->n_philos)
	{
		init_one_philo(&(*philos)[i], i, forks, rules);
		i++;
	}
	return (0);
}

int	init_all(t_philo **philos, pthread_mutex_t **forks, t_rules *rules)
{
	if (init_forks(forks, rules))
		return (1);
	if (init_philos(philos, *forks, rules))
		return (1);
	return (0);
}
