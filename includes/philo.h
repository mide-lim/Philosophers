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
	int	nbr_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
}	t_args;

/* ---------- Parsing ---------- */
int		parse_args(int argc, char **argv, t_args *args);

/* ---------- Time Utils ---------- */
long	get_time_ms(void);
void	ft_usleep(long ms);

#endif

