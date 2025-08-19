#include "philo.h"

int main(int argc, char **argv)
{
    int i;
    t_args data;
    t_philo *philos;

    if (parse_args(argc, argv, &data) != 0)
        return (1);
    philos = malloc(sizeof(t_philo) *data.number_of_philosophers);
    if (!philos)
        return(1);
    i = 0;
    while (i < data.number_of_philosophers)
    {
        philos[i].id = i + 1;
        philos[i].data = &data;
        pthread_create(&philos[i].thread_id, NULL, philo_routine, &philos[i]);
        i++;
    }
    i = 0;
    while (i < data.number_of_philosophers)
    {
        pthread_join(philos[i].thread_id, NULL);
        i++;
    }
    free(philos);
    return (0);
}
