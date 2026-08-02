#include "philo.h"

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, data))
		return (1);
	join_threads(data);
	pthread_join(monitor, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (!valid_args(argc, argv))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (!init_data(&data, argc, argv))
	{
		printf("Error: initialization failed\n");
		return (1);
	}
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
