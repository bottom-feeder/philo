/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 23:34:28 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/27 06:13:54 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_pickup_routine(t_philo *ph)
{
	if (ph->rf > ph->lf || ph->num_of_phs == 1)
		ph->l_first = 1;
	if (take_1st_fork(ph, ph->l_first))
		return (1);
	if (take_2nd_fork(ph, ph->l_first))
		return (1);
	return (0);
}

int	eat_routine(t_philo *ph)
{
	pthread_mutex_lock(ph->mlck);
	// printf("Locked mlck for philo %d from eat routine\n", ph->id);
	size_t	now = gct();
	if (check_starvation(ph, now) == 0)
	{
		ph->l_meal = gct();
		// printf("Updated last_meal_time for philo %d\n", ph->id);
	}
	// ph->l_meal = gct();
	// printf("Updated last_meal_time for philo %d\n", ph->id);
	else
	{
		pthread_mutex_unlock(ph->mlck);
		pthread_mutex_unlock(ph->rf);
		pthread_mutex_unlock(ph->lf);
		return (1);
	}
	pthread_mutex_unlock(ph->mlck);
	if (poll_death(ph) == 0)
	{
		pthread_mutex_lock(ph->wlck);
		printf("%zu %d is eating\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
	{	
		pthread_mutex_unlock(ph->rf);
		pthread_mutex_unlock(ph->lf);
		return (1);
	}
		//return (pthread_mutex_unlock(ph->wlck), 1);
	if (ft_usleep(ph->t2e, ph) == 1)
	{
		pthread_mutex_unlock(ph->rf);
		pthread_mutex_unlock(ph->lf);
		return (1);
	}
	if (ph->l_first)
	{
		pthread_mutex_unlock(ph->rf);
		pthread_mutex_unlock(ph->lf);
	}
	else
	{
		pthread_mutex_unlock(ph->lf);
		pthread_mutex_unlock(ph->rf);
	}
	pthread_mutex_lock(ph->mlck);
	if (++ph->meals_eaten == ph->num_times_to_eat)
		//return (pthread_mutex_unlock(ph->mlck), -1);
	{	
		//pthread_mutex_lock(ph->mlck);
		ph->stop = 1;
		return (pthread_mutex_unlock(ph->mlck), 1);
	}
	return (pthread_mutex_unlock(ph->mlck), 0);
	
	return (0);
}

int	sleep_routine(t_philo *ph)
{
	
	if (poll_death(ph) == 0)
	{
		pthread_mutex_lock(ph->wlck);
		printf("%zu %d is sleeping\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (1);
	if (ft_usleep(ph->t2s, ph) == 1)
		return (1);
	return (0);
}

int	think_routine(t_philo *ph)
{
	
	if (poll_death(ph) == 0)
	{
		pthread_mutex_lock(ph->wlck);
		printf("%zu %d is thinking\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (1);
	if (ph->id % 2 == 1)
	{
		// size_t think_time = ph->t2e / 2;
		if (ft_usleep(1, ph) == 1)
			return (1);
	}
	return (0);
}

void	*ph_routine(void *arg)
{
	t_philo	*ph;

	ph = arg;
	if (ph->id % 2 == 0)
		ft_usleep(1, ph);
	while (poll_death(ph) == 0)
	{
		if (fork_pickup_routine(ph) == 1)
			return (NULL);
		if (eat_routine(ph) == 1)
			return (NULL);
		if (sleep_routine(ph) == 1)
			return (NULL);
		if (think_routine(ph) == 1)
			return (NULL);
	}
	return (NULL);
}
