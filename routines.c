/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 23:34:28 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 21:12:20 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fork_pickup_routine(t_philo *ph)
{
	int	l_first;

	l_first = 0;
	if (ph->rf > ph->lf || ph->num_of_phs == 1)
		l_first = 1;
	if (take_1st_fork(ph, l_first))
		return (1);
	if (take_2nd_fork(ph, l_first))
		return (1);
	return (0);
}

int	eat_routine(t_philo *ph)
{
	pthread_mutex_lock(ph->l_meal_lock);
	ph->l_meal = gct();
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		pthread_mutex_unlock(ph->l_meal_lock);
		printf("%zu %d is eating\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	if (ft_usleep(ph->t2e, ph) == 1)
	{
		pthread_mutex_unlock(ph->l_meal_lock);
		pthread_mutex_unlock(ph->wlck);
		pthread_mutex_unlock(ph->rf);
		pthread_mutex_unlock(ph->lf);
		return (1);
	}
	pthread_mutex_unlock(ph->rf);
	pthread_mutex_unlock(ph->lf);
	pthread_mutex_lock(ph->l_meal_lock);
	ph->meals_eaten++;
	if (ph->meals_eaten == ph->num_times_to_eat)
		return (pthread_mutex_unlock(ph->l_meal_lock), -1);
	return (pthread_mutex_unlock(ph->l_meal_lock), 0);
}

int	sleep_routine(t_philo *ph)
{
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d is sleeping\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	if (ft_usleep(ph->t2s, ph) == 1)
		return (pthread_mutex_unlock(ph->wlck), 1);
	return (0);
}

int	think_routine(t_philo *ph)
{
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d is thinking\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (pthread_mutex_unlock(ph->wlck), 1);
	return (0);
}

void	*ph_routine(void *arg)
{
	t_philo	*ph;
	int		ate;

	ph = arg;
	if (ph->id % 2 == 0)
		usleep(300);
	while (poll_death(ph) == 0)
	{
		if (fork_pickup_routine(ph) != 0)
			return (NULL);
		ate = eat_routine(ph);
		if (ate != 0)
		{
			pthread_mutex_lock(ph->l_meal_lock);
			if (ate == -1)
				ph->stop = 1;
			pthread_mutex_unlock(ph->l_meal_lock);
			return (NULL);
		}
		if (sleep_routine(ph) == 1)
			return (NULL);
		if (think_routine(ph) == 1)
			return (NULL);
	}
	return (NULL);
}
