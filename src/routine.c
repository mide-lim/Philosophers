#include "philo.h"

static void	take_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->right_fork);
		print_state(p, "has taken a fork");
		pthread_mutex_lock(p->left_fork);
		print_state(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(p->left_fork);
		print_state(p, "has taken a fork");
		pthread_mutex_lock(p->right_fork);
		print_state(p, "has taken a fork");
	}
}

static void	drop_forks(t_philo *p)
{
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
}

static void	philo_eat(t_philo *p)
{
	pthread_mutex_lock(&p->rules->meal_mutex);
	p->last_meal = get_time_ms();
	p->meals_eaten++;
	pthread_mutex_unlock(&p->rules->meal_mutex);
	print_state(p, "is eating");
	precise_sleep(p->rules->t_eat, p->rules);
}

static void	philo_sleep_think(t_philo *p)
{
	long long	think_time;

	print_state(p, "is sleeping");
	precise_sleep(p->rules->t_sleep, p->rules);
	print_state(p, "is thinking");
	if (p->rules->n_philos % 2 != 0)
	{
		think_time = (p->rules->t_eat * 2) - p->rules->t_sleep;
		if (think_time < 0)
			think_time = 0;
		if (think_time > 0)
			precise_sleep(think_time, p->rules);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		usleep(1000);
	while (!is_dead(p->rules))
	{
		take_forks(p);
		if (is_dead(p->rules))
		{
			drop_forks(p);
			break ;
		}
		philo_eat(p);
		drop_forks(p);
		if (p->rules->must_eat > 0 && p->meals_eaten >= p->rules->must_eat)
			break ;
		philo_sleep_think(p);
	}
	return (NULL);
}
