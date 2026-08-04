/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darafael <darafael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:05:37 by darafael          #+#    #+#             */
/*   Updated: 2026/08/04 20:05:39 by darafael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	t_fork	*first;
	t_fork	*second;

	if (philo->id % 2 == 0)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	else
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	pthread_mutex_lock(&first->mutex);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&second->mutex);
	print_status(philo, "has taken a fork");
}

void	eat(t_philo *philo)
{
	if (sim_stopped(philo->data))
		return ;
	take_forks(philo);
	if (sim_stopped(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		return ;
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

void	philo_sleep(t_philo *philo)
{
	long long	think;

	if (sim_stopped(philo->data))
		return ;
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	if (sim_stopped(philo->data))
		return ;
	print_status(philo, "is thinking");
	think = philo->data->time_to_die - philo->data->time_to_eat
		- philo->data->time_to_sleep - 20;
	if (philo->data->num_philos % 2 != 0 && think > 0)
		ft_usleep(think / 2);
}

void	one_philo(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	while (!sim_stopped(philo->data))
		usleep(1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!sim_stopped(philo->data))
	{
		eat(philo);
		philo_sleep(philo);
	}
	return (NULL);
}
