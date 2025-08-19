#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

/* ---------- Struct ---------- */
typedef struct s_args
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
	int	stop;
	int	start_time;
	pthread_mutex_t	*forks;
}	t_args;

typedef struct s_philo
{
	int	id;
	int	meals_eaten;
	int	last_meal;
	int	left_fork;
	int	right_fork;
	t_args	*data;
	pthread_t	thread_id;
}	t_philo;

/* ---------- Parsing ---------- */
int		parse_args(int argc, char **argv, t_args *args);
void    *philo_routine(void *arg);

/* ---------- Time Utils ---------- */
long	get_time_ms(void);
void	ft_usleep(long ms);

#endif

