/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:10:37 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 21:14:33 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_1st_fork(t_philo *ph, int l_first)
{
	if (l_first)
		pthread_mutex_lock(ph->lf);
	else
		pthread_mutex_lock(ph->rf);
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d has taken a fork\n", gct() - ph->start_time, ph->id);
		pthread_mutex_unlock(ph->wlck);
	}
	else
	{
		pthread_mutex_unlock(ph->wlck);
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
	else
		pthread_mutex_lock(ph->lf);
	pthread_mutex_lock(ph->wlck);
	if (poll_death(ph) == 0)
	{
		printf("%zu %d has taken a fork\n", gct() - ph->start_time, ph->id);
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
