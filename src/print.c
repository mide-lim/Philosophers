#include "philo.h"

void	print_state(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->rules->write_mutex);
	if (!is_dead(philo->rules))
	{
		time = elapsed_ms(philo->rules->start_time);
		printf("%lld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->write_mutex);
}

void	print_death(t_philo *philo)
{
	long long	time;

	pthread_mutex_lock(&philo->rules->write_mutex);
	if (!is_dead(philo->rules))
	{
		time = elapsed_ms(philo->rules->start_time);
		printf("%lld %d died\n", time, philo->id);
	}
	pthread_mutex_unlock(&philo->rules->write_mutex);
}
