#include "philo.h"

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL))
			return (0);
		i++;
	}
	return (1);
}

void	assign_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		i++;
	}
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = 0;
		data->philos[i].data = data;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL))
			return (0);
		i++;
	}
	assign_forks(data);
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->num_philos = ft_atoi(argv[1], NULL);
	data->time_to_die = ft_atoi(argv[2], NULL);
	data->time_to_eat = ft_atoi(argv[3], NULL);
	data->time_to_sleep = ft_atoi(argv[4], NULL);
	data->must_eat = 0;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5], NULL);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (0);
	if (!init_forks(data))
		return (0);
	if (!init_philos(data))
		return (0);
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (data->forks && i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		i++;
	}
	i = 0;
	while (data->philos && i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(data->philos);
}
