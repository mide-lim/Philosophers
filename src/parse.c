#include "philo.h"

static int	set_rules(int ac, char **av, t_rules *rules)
{
	rules->n_philos = ft_atoi(av[1]);
	rules->t_die = ft_atoi(av[2]);
	rules->t_eat = ft_atoi(av[3]);
	rules->t_sleep = ft_atoi(av[4]);
	rules->must_eat = -1;
	if (ac == 6)
		rules->must_eat = ft_atoi(av[5]);
	if (rules->n_philos <= 0 || rules->t_die <= 0 || rules->t_eat <= 0
		|| rules->t_sleep <= 0)
		return (1);
	if (ac == 6 && rules->must_eat <= 0)
		return (1);
	return (0);
}

int	parse_args(int ac, char **av, t_rules *rules)
{
	int	i;

	if (ac != 5 && ac != 6)
	{
		write(2, "Error: wrong number of args\n", 28);
		return (1);
	}
	i = 1;
	while (i < ac)
	{
		if (!ft_isdigit_str(av[i]))
		{
			write(2, "Error: non numeric arg\n", 23);
			return (1);
		}
		i++;
	}
	if (set_rules(ac, av, rules))
	{
		write(2, "Error: invalid args value\n", 26);
		return (1);
	}
	return (0);
}
