/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_starv_stagger.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 08:08:51 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/28 22:58:39 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_starvation(t_philo *ph, size_t now)
{
	size_t	interval;

	interval = now - ph->l_meal;
	if (interval > ph->t2d && ph->stop == 0)
		return (1);
	return (0);
}

int	thinking_stagger_odd(t_philo *ph)
{
	if (ph->num_of_phs < 50)
	{
		if (ft_usleep(ph->t2e * 2 - ph->t2s, ph) == 1)
			return (1);
	}
	else
	{
		if (ft_usleep(ph->t2e / 40 + (ph->id % 10), ph) == 1)
			return (1);
	}
	return (0);
}

int	thinking_stagger_even(t_philo *ph)
{
	size_t	slack_time;
	size_t	think_time;

	slack_time = ph->t2d - ph->t2e - ph->t2s;
	if (ph->num_of_phs < 50)
	{
		if (ft_usleep(1, ph) == 1)
			return (1);
	}
	else
	{
		think_time = 1 + (ph->id % 2);
		if (think_time > slack_time)
			think_time = slack_time;
		if (ft_usleep(think_time, ph) == 1)
			return (1);
	}
	return (0);
}
