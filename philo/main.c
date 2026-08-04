/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darafael <darafael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:05:03 by darafael          #+#    #+#             */
/*   Updated: 2026/08/04 20:05:05 by darafael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_threads(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

static void	abort_simulation(t_data *data, int count)
{
	pthread_mutex_lock(&data->print_mutex);
	data->dead = 1;
	pthread_mutex_unlock(&data->print_mutex);
	join_threads(data, count);
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
		{
			abort_simulation(data, i);
			return (1);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, data))
	{
		abort_simulation(data, data->num_philos);
		return (1);
	}
	join_threads(data, data->num_philos);
	pthread_join(monitor, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	setbuf(stdout, NULL);
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
		cleanup(&data);
		return (1);
	}
	if (start_simulation(&data))
		printf("Error: thread creation failed\n");
	cleanup(&data);
	return (0);
}
