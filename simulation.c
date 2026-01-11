/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:36:51 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 21:12:50 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_death(t_program *pr, int i)
{
	pthread_mutex_unlock(&pr->l_meal_lock);
	announce_death(&pr->phs[i]);
	pthread_mutex_lock(pr->phs[i].wlck);
	pthread_mutex_lock(pr->phs[i].dlck);
	printf("%zu %d died\n", gct() - pr->phs[i].start_time, pr->phs[i].id);
	pthread_mutex_unlock(pr->phs[i].dlck);
	pthread_mutex_unlock(pr->phs[i].wlck);
}

int	poll_death(t_philo *ph)
{
	int	d;

	pthread_mutex_lock(ph->dlck);
	d = *ph->dead;
	pthread_mutex_unlock(ph->dlck);
	return (d);
}

void	announce_death(t_philo *ph)
{
	pthread_mutex_lock(ph->dlck);
	*ph->dead = 1;
	pthread_mutex_unlock(ph->dlck);
}

static void	all_eaten(t_program *pr, int i)
{
	pthread_mutex_unlock(&pr->l_meal_lock);
	announce_death(&pr->phs[i]);
}

void	*monitor(void *arg)
{
	t_program	*pr;
	int			ctr;
	int			i;

	pr = arg;
	i = 0;
	ctr = 0;
	while (poll_death(&pr->phs[i]) == 0)
	{
		pthread_mutex_lock(&pr->l_meal_lock);
		if ((gct() - pr->phs[i].l_meal > pr->phs[i].t2d) && (!pr->phs[i].stop))
			return (print_death(pr, i), NULL);
		if (pr->phs[i].stop == 1)
		{
			ctr++;
			pr->phs[i].stop = -1;
			if (ctr > 0 && ctr == pr->num_of_phs)
				return (all_eaten(pr, i), NULL);
		}
		pthread_mutex_unlock(&pr->l_meal_lock);
		if (++i == pr->num_of_phs)
			i = 0;
		usleep(300);
	}
	return (NULL);
}
