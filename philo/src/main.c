#include "philo.h"

int main(int argc, char **argv)
{
    t_config config;

    if (parse_arguments(argc, argv, &config) != 0)
        return (1);

    printf("Parsed arguments:\n");
    printf(" Philosophers: %d\n", config.philosophers);
    printf(" Time to die: %d\n", config.time_to_die);
    printf(" Time to eat: %d\n", config.time_to_eat);
    printf(" Time to sleep: %d\n", config.time_to_sleep);
    if (config.must_eat_count != -1)
        printf(" Must eat count: %d\n", config.must_eat_count);
    else
        printf(" Must eat count: (not provided)\n");

    return (0);
}
