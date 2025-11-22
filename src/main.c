#include "philo.h"

static int	init_rules_mutexes(t_rules *rules)
{
	if (pthread_mutex_init(&rules->write_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&rules->dead_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&rules->meal_mutex, NULL))
		return (1);
	rules->someone_died = 0;
	return (0);
}

static int	start_threads(t_philo *philos, t_rules *rules)
{
	int	i;

	rules->start_time = get_time_ms();
	i = 0;
	while (i < rules->n_philos)
	{
		philos[i].last_meal = rules->start_time;
		if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	join_threads(t_philo *philos, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->n_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_rules			rules;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (parse_args(ac, av, &rules))
		return (1);
	if (init_rules_mutexes(&rules))
		return (1);
	if (init_all(&philos, &forks, &rules))
		return (1);
	if (rules.n_philos == 1)
	{
		print_state(&philos[0], "has taken a fork");
		precise_sleep(rules.t_die, &rules);
		print_death(&philos[0]);
	}
	else
	{
		if (start_threads(philos, &rules))
			return (1);
		monitor_loop(philos, &rules);
		join_threads(philos, &rules);
	}
	cleanup_all(philos, forks, &rules);
	return (0);
}
