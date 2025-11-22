#include "philo.h"

static int	all_ate_enough(t_philo *philos, t_rules *rules)
{
	int	i;
	int	done;

	if (rules->must_eat <= 0)
		return (0);
	i = 0;
	done = 0;
	pthread_mutex_lock(&rules->meal_mutex);
	while (i < rules->n_philos)
	{
		if (philos[i].meals_eaten >= rules->must_eat)
			done++;
		i++;
	}
	pthread_mutex_unlock(&rules->meal_mutex);
	if (done == rules->n_philos)
		return (1);
	return (0);
}

void	monitor_loop(t_philo *philos, t_rules *rules)
{
	int			i;
	long long	now;

	while (!is_dead(rules))
	{
		if (all_ate_enough(philos, rules))
			break ;
		i = 0;
		while (i < rules->n_philos && !is_dead(rules))
		{
			pthread_mutex_lock(&rules->meal_mutex);
			now = get_time_ms();
			if (now - philos[i].last_meal >= rules->t_die)
			{
				pthread_mutex_unlock(&rules->meal_mutex);
				print_death(&philos[i]);
				set_dead(rules);
				break ;
			}
			pthread_mutex_unlock(&rules->meal_mutex);
			i++;
		}
		usleep(500);
	}
}
