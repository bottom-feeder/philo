/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 23:34:28 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 19:52:30 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_pickup_routine(t_philo *philo)
{
	int l_first;
	
	l_first = 0;
	if (philo->r_fork > philo->l_fork || philo->num_of_philos == 1)
		(pthread_mutex_lock(philo->l_fork), l_first = 1);
	else
		pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(philo->write_lock);	
	if (poll_death(philo) == 0)
	{
		printf("%zu %d has taken a fork\n", get_current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->write_lock);
	}
	else
	{
		if (l_first)
			return(pthread_mutex_unlock(philo->write_lock), pthread_mutex_unlock(philo->l_fork), 1);
		return(pthread_mutex_unlock(philo->write_lock), pthread_mutex_unlock(philo->r_fork), 1);
	}
	if (philo->num_of_philos == 1)
		return (pthread_mutex_unlock(philo->l_fork), -1);
	if (l_first)
		pthread_mutex_lock(philo->r_fork);
	else
		pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->write_lock);
	if (poll_death(philo) == 0)
	{
		printf("%zu %d has taken a fork\n", get_current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->write_lock);
	}
	else
		return(pthread_mutex_unlock(philo->write_lock), pthread_mutex_unlock(philo->l_fork), pthread_mutex_unlock(philo->r_fork), 1);
	return (0);
}

int	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->last_meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_lock(philo->write_lock);
	if (poll_death(philo) == 0)
	{
		pthread_mutex_unlock(philo->last_meal_lock);
		printf("%zu %d is eating\n", get_current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->write_lock);
	}
	if (ft_usleep(philo->time_to_eat, philo) == 1)
		return (pthread_mutex_unlock(philo->last_meal_lock), pthread_mutex_unlock(philo->write_lock), pthread_mutex_unlock(philo->r_fork), pthread_mutex_unlock(philo->l_fork), 1);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_lock(philo->last_meal_lock);
	philo->meals_eaten++;
	if (philo->meals_eaten == philo->num_times_to_eat)
		return(pthread_mutex_unlock(philo->last_meal_lock), -1);
	return (pthread_mutex_unlock(philo->last_meal_lock), 0);
}

int	sleep_routine(t_philo *philo)
{	
	pthread_mutex_lock(philo->write_lock);
	if (poll_death(philo) == 0)
	{
		printf("%zu %d is sleeping\n", get_current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->write_lock);
	}
	if (ft_usleep(philo->time_to_sleep, philo) == 1)
		return (pthread_mutex_unlock(philo->write_lock), 1);
	return (0);
}

int think_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->write_lock);
	if (poll_death(philo) == 0)
	{
		printf("%zu %d is thinking\n", get_current_time() - philo->start_time, philo->id);
		pthread_mutex_unlock(philo->write_lock);
	}
	else
		return(pthread_mutex_unlock(philo->write_lock), 1);
	return (0);
}

void *philo_routine(void *arg)
{
	t_philo *philo;
	int		ate;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(300);
	while (poll_death(philo) == 0)
	{
		if (fork_pickup_routine(philo) == 1)
			return (NULL);
		ate = eat_routine(philo);
		if (ate != 0)
		{
			pthread_mutex_lock(philo->last_meal_lock);
			if (ate == -1)
				philo->stopped = 1;
			pthread_mutex_unlock(philo->last_meal_lock);
			return (NULL);
		}
		if (sleep_routine(philo) == 1)
			return (NULL);
		if (think_routine(philo) == 1)
			return (NULL);
	}
	return (NULL);
}
