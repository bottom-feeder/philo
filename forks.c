/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:10:37 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/27 06:49:03 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_1st_fork(t_philo *ph, int l_first)
{
	if (l_first)
		pthread_mutex_lock(ph->lf);
	else
		pthread_mutex_lock(ph->rf);
	if (poll_death(ph) == 0)
	{
		pthread_mutex_lock(ph->wlck);
		if (l_first)
			printf("%zu %d has taken left fork\n", gct() - ph->start_time, ph->id);
		else
			printf("%zu %d has taken right fork\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
	{
		if (l_first)
		{
			pthread_mutex_unlock(ph->lf);
			return (1);
		}
		pthread_mutex_unlock(ph->rf);
		return (1);
	}
	if (ph->num_of_phs == 1)
		return (pthread_mutex_unlock(ph->lf), 1);
	return (0);
}

int	take_2nd_fork(t_philo *ph, int l_first)
{
	if (l_first)
		pthread_mutex_lock(ph->rf);
		// pthread_mutex_lock(ph->lf);
	else
		pthread_mutex_lock(ph->lf);
		// pthread_mutex_lock(ph->rf);
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		if (l_first)
			printf("%zu %d has taken right fork\n", gct() - ph->start_time, ph->id);
		else
			printf("%zu %d has taken left fork\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
	{
		pthread_mutex_unlock(ph->wlck);
		pthread_mutex_unlock(ph->lf);
		pthread_mutex_unlock(ph->rf);
		return (1);
	}
	return (0);
}

int	check_starvation(t_philo *ph, size_t now)
{
	size_t interval = now - ph->l_meal;
	if (interval >= ph->t2d && ph->stop == 0)
	{
		printf("Philo %d starved bc %zu > %zu\n", ph->id, interval, ph->t2d);
		return (1);
	}
	printf("No starvation for %d: %zu < %zu\n", ph->id, interval, ph->t2d);
	return (0);
}
