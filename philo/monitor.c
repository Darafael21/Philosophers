/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darafael <darafael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:05:19 by darafael          #+#    #+#             */
/*   Updated: 2026/08/04 20:05:21 by darafael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sim_stopped(t_data *data)
{
	int	stopped;

	pthread_mutex_lock(&data->print_mutex);
	stopped = data->dead || data->all_ate;
	pthread_mutex_unlock(&data->print_mutex);
	return (stopped);
}

void	kill_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->dead && !philo->data->all_ate)
	{
		philo->data->dead = 1;
		printf("%lld %d died\n",
			get_time() - philo->data->start_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	check_philo_death(t_philo *philo)
{
	long long	last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (get_time() - last_meal >= philo->data->time_to_die)
	{
		kill_philo(philo);
		return (1);
	}
	return (0);
}

int	check_all_ate(t_data *data)
{
	int	i;
	int	meals;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		meals = data->philos[i].meals_eaten;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (meals < data->must_eat)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!sim_stopped(data))
	{
		i = 0;
		while (i < data->num_philos && !sim_stopped(data))
		{
			if (check_philo_death(&data->philos[i]))
				return (NULL);
			i++;
		}
		if (data->must_eat > 0 && check_all_ate(data))
		{
			pthread_mutex_lock(&data->print_mutex);
			data->all_ate = 1;
			pthread_mutex_unlock(&data->print_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
