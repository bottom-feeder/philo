/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 23:34:28 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/28 08:16:05 by ikiriush         ###   ########.fr       */
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
	size_t	now;

	pthread_mutex_lock(ph->mlck);
	now = gct();
	if (check_starvation(ph, now) == 0)
		ph->l_meal = now;
	else
		return (pthread_mutex_unlock(ph->mlck), put_down_forks(ph), 1);
	pthread_mutex_unlock(ph->mlck);
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d is eating\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (pthread_mutex_unlock(ph->wlck), put_down_forks(ph), 1);
	if (ft_usleep(ph->t2e, ph) == 1)
		return (put_down_forks(ph), 1);
	put_down_forks(ph);
	pthread_mutex_lock(ph->mlck);
	if (++ph->meals_eaten == ph->num_times_to_eat)
		return (ph->stop = 1, pthread_mutex_unlock(ph->mlck), 1);
	return (pthread_mutex_unlock(ph->mlck), 0);
}

int	sleep_routine(t_philo *ph)
{
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d is sleeping\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (pthread_mutex_unlock(ph->wlck), 1);
	if (ft_usleep(ph->t2s, ph) == 1)
		return (1);
	return (0);
}

int	think_routine(t_philo *ph)
{
	size_t	think_time;

	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d is thinking\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
		return (pthread_mutex_unlock(ph->wlck), 1);
	think_time = ph->t2e * 2 - ph->t2s;
	if (ph->num_of_phs % 2 == 1)
	{
		if (thinking_stagger_odd(ph) == 1)
			return (1);
	}
	else if (ph->num_of_phs % 2 == 0)
	{
		if (thinking_stagger_even(ph) == 1)
			return (1);
	}
	return (0);
}

void	*ph_routine(void *arg)
{
	t_philo	*ph;

	ph = arg;
	if (ph->id % 2 == 0)
		ft_usleep(20, ph);
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
