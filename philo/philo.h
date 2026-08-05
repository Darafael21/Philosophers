/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darafael <darafael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 20:05:28 by darafael          #+#    #+#             */
/*   Updated: 2026/08/05 07:36:22 by darafael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data	t_data;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_data			*data;
	pthread_mutex_t	meal_mutex;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat;
	int				dead;
	int				all_ate;
	long long		start_time;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	print_mutex;
	int				print_mutex_ok;
}	t_data;


long long	get_time(void);
void		ft_usleep(long long ms, t_data *data);
void		print_status(t_philo *philo, char *status);
int			ft_atoi(const char *str, int *overflow);
int			valid_args(int argc, char **argv);
int			init_data(t_data *data, int argc, char **argv);
int			init_forks(t_data *data);
int			init_philos(t_data *data);
void		assign_forks(t_data *data);
void		cleanup(t_data *data);
void		*philo_routine(void *arg);
void		eat(t_philo *philo);
void		take_forks_not_wars(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		lonely_guy(t_philo *philo);
void		*monitor_routine(void *arg);
int			sim_stopped(t_data *data);
int			check_philo_death(t_philo *philo);
int			check_all_ate(t_data *data);
void		kill_philo(t_philo *philo);
int			start_simulation(t_data *data);

#endif
