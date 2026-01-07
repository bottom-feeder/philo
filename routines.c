/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 23:34:28 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/08 03:25:06 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_pickup_routine(t_philo *philo)
{
	//printf("%s%d started fork routine%s\n", philo_color(philo->id), philo->id, C_RESET);
	int l_first;
	
	l_first = 0;
	if (philo->r_fork > philo->l_fork || philo->num_of_philos == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		//printf("%s%d locked 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET);
		if (poll_death(philo))	
			// return(pthread_mutex_unlock(philo->l_fork), //printf("%s%d released 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET), 1);
			return(pthread_mutex_unlock(philo->l_fork), 1);
		l_first = 1;
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		//printf("%s%d locked up 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET);
		if (poll_death(philo))
			// return(pthread_mutex_unlock(philo->r_fork), //printf("%s%d released 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET), 1);
			return(pthread_mutex_unlock(philo->r_fork), 1);
		}
	pthread_mutex_lock(philo->write_lock);
	//printf("%s%d write locked [Fork routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	printf("%s%zu %d has taken a fork%s\n", philo_color(philo->id), get_current_time() - philo->start_time, philo->id, C_RESET);
	pthread_mutex_unlock(philo->write_lock);
	//printf("%s%d write unlocked [Fork routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	if (philo->num_of_philos == 1)
		return (pthread_mutex_unlock(philo->l_fork), -1);
	if (l_first)
	{
		pthread_mutex_lock(philo->r_fork);
		//printf("%s%d picked up 2nd fork%s\n", philo_color(philo->id), philo->id, C_RESET);
		if (poll_death(philo))
			// return(pthread_mutex_unlock(philo->l_fork), //printf("%s%d released 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET), pthread_mutex_unlock(philo->r_fork), //printf("%s%d released 2nd fork%s\n", philo_color(philo->id), philo->id, C_RESET), 1);
			return(pthread_mutex_unlock(philo->l_fork), pthread_mutex_unlock(philo->r_fork), 1);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		// //printf("%s%d locked 2nd fork%s\n", philo_color(philo->id), philo->id, C_RESET);
		if (poll_death(philo))
			// return(pthread_mutex_unlock(philo->r_fork), //printf("%s%d released 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET), pthread_mutex_unlock(philo->l_fork), //printf("%s%d released 2nd fork%s\n", philo_color(philo->id), philo->id, C_RESET), 1);
			return(pthread_mutex_unlock(philo->r_fork), pthread_mutex_unlock(philo->l_fork), 1);
	}
	pthread_mutex_lock(philo->write_lock);
	//printf("%s%d write locked [Fork routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	printf("%s%zu %d has taken a fork%s\n", philo_color(philo->id), get_current_time() - philo->start_time, philo->id, C_RESET);
	// return (pthread_mutex_unlock(philo->write_lock), //printf("%s%d write unlocked [Fork routine]%s\n", philo_color(philo->id), philo->id, C_RESET), 0);
	return (pthread_mutex_unlock(philo->write_lock), 0);
}

int	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->last_meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(philo->last_meal_lock);
	//printf("%s%d started eat routine%s\n", philo_color(philo->id), philo->id, C_RESET);
	pthread_mutex_lock(philo->write_lock);
	//printf("%s%d write locked [Eat routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	printf("%s%zu %d is eating%s\n", philo_color(philo->id), get_current_time() - philo->start_time, philo->id, C_RESET);
	pthread_mutex_unlock(philo->write_lock);
	//printf("%s%d write unlocked [Eat routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	if (ft_usleep(philo->time_to_eat, philo) == 1)
		return (announce_death(philo), pthread_mutex_unlock(philo->r_fork), pthread_mutex_unlock(philo->l_fork), 1);
	pthread_mutex_unlock(philo->r_fork);
	//printf("%s%d unlocked 1st fork%s\n", philo_color(philo->id), philo->id, C_RESET);
	pthread_mutex_unlock(philo->l_fork);
	//printf("%s%d unlocked 2nd fork%s\n", philo_color(philo->id), philo->id, C_RESET);
	pthread_mutex_lock(philo->last_meal_lock);
	philo->meals_eaten++;
	//printf("%s%d ate %d times out of %d%s\n", philo_color(philo->id), philo->id, philo->meals_eaten, philo->num_times_to_eat, C_RESET);
	if (philo->meals_eaten == philo->num_times_to_eat)
		// return(pthread_mutex_unlock(philo->last_meal_lock), //printf ("Match! Returning -1\n"), -1);
		return(pthread_mutex_unlock(philo->last_meal_lock), -1);
	pthread_mutex_unlock(philo->last_meal_lock);
	return (0);
}

int	sleep_routine(t_philo *philo)
{	
	//printf("%s%d started sleep routine%s\n", philo_color(philo->id), philo->id, C_RESET);
	pthread_mutex_lock(philo->write_lock);
	//printf("%s%d write locked [Sleep routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	printf("%s%zu %d is sleeping%s\n", philo_color(philo->id), get_current_time() - philo->start_time, philo->id, C_RESET);
	pthread_mutex_unlock(philo->write_lock);
	//printf("%s%d write unlocked [Sleep routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	if (ft_usleep(philo->time_to_sleep, philo))
		return (announce_death(philo), 1);
	return (0);
}

void think_routine(t_philo *philo)
{
	//printf("%s%d started think routine%s\n", philo_color(philo->id), philo->id, C_RESET);
	pthread_mutex_lock(philo->write_lock);
	//printf("%s%d write locked [Think routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
	printf("%s%zu %d is thinking%s\n", philo_color(philo->id), get_current_time() - philo->start_time, philo->id, C_RESET);
	pthread_mutex_unlock(philo->write_lock);
	//printf("%s%d write unlocked [Think routine]%s\n", philo_color(philo->id), philo->id, C_RESET);
}

void *philo_routine(void *arg)
{
	t_philo *philo;
	int		ate;
	int		fork;

	philo = arg;
	ate = 0;
	fork = 0;
	//printf ("Philo routine %d started\n", philo->id);
	// if (philo->id % 2 == 0)
	// 	ft_usleep(1, philo);
	while (poll_death(philo) == 0)
	{
		if (poll_death(philo))
			return(announce_death(philo), NULL);
		else
		{
			fork = fork_pickup_routine(philo);
			{
				if (fork  == 1)
					return(announce_death(philo), NULL);
				else if (fork == -1)
					break ;
			}
		}
		if (poll_death(philo))
			return(announce_death(philo), NULL);
		else
		{
			ate = eat_routine(philo);
			if (ate == 1)
				return(announce_death(philo), NULL);
			else if (ate == -1)
			{
				pthread_mutex_lock(philo->last_meal_lock);
				philo->stopped = 1;
				pthread_mutex_unlock(philo->last_meal_lock);
				break ;
			}
		}
		if (poll_death(philo))
			return(announce_death(philo), NULL);
		else
			if (sleep_routine(philo) == 1)
				return(announce_death(philo), NULL);
		if (poll_death(philo))
			return(announce_death(philo), NULL);
		else
			think_routine(philo);
	}
	//printf ("Philo routine %d ended\n", philo->id);
	return (NULL);
}
