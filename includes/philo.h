#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_rules
{
	int				n_philos;
	long long		t_die;
	long long		t_eat;
	long long		t_sleep;
	int				must_eat;
	long long		start_time;
	int				someone_died;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	meal_mutex;
}	t_rules;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
}	t_philo;

/* parse.c */
int			parse_args(int ac, char **av, t_rules *rules);

/* init.c */
int			init_all(t_philo **philos, pthread_mutex_t **forks, t_rules *rules);

/* routine.c */
void		*philo_routine(void *arg);

/* monitor.c */
void		monitor_loop(t_philo *philos, t_rules *rules);

/* print.c */
void		print_state(t_philo *philo, const char *msg);
void		print_death(t_philo *philo);

/* utils.c */
long long	get_time_ms(void);
long long	elapsed_ms(long long since);
void		precise_sleep(long long ms, t_rules *rules);
int			ft_atoi(const char *s);
int			ft_isdigit_str(const char *s);
int			is_dead(t_rules *rules);
void		set_dead(t_rules *rules);

/* cleanup.c */
void		cleanup_all(t_philo *philos, pthread_mutex_t *forks, t_rules *rules);

#endif
