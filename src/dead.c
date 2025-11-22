#include "philo.h"

int	is_dead(t_rules *rules)
{
	int	dead;

	pthread_mutex_lock(&rules->dead_mutex);
	dead = rules->someone_died;
	pthread_mutex_unlock(&rules->dead_mutex);
	return (dead);
}

void	set_dead(t_rules *rules)
{
	pthread_mutex_lock(&rules->dead_mutex);
	rules->someone_died = 1;
	pthread_mutex_unlock(&rules->dead_mutex);
}
