#include "philo.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long long	elapsed_ms(long long since)
{
	return (get_time_ms() - since);
}

void	precise_sleep(long long ms, t_rules *rules)
{
	long long	start;

	start = get_time_ms();
	while (!is_dead(rules) && get_time_ms() - start < ms)
		usleep(500);
}

int	ft_atoi(const char *s)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return ((int)res);
}

int	ft_isdigit_str(const char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
