/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:36:51 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/10 18:29:16 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



int poll_death(t_philo *philo)
{
	int	d;

	pthread_mutex_lock(philo->death_lock);
	d = *philo->dead;
	pthread_mutex_unlock(philo->death_lock);
	return (d);
}

void announce_death(t_philo *philo)
{
	pthread_mutex_lock(philo->death_lock);
	*philo->dead = 1;
	philo->time_died = get_current_time() - philo->start_time;
	pthread_mutex_unlock(philo->death_lock);
}

void *monitor(void *arg)
{
	t_program	*program;
	int			ctr;
	int			i;

	program = arg;
	i = 0;
	ctr = 0;
	while(poll_death(&program->philos[i]) == 0)
	{
		pthread_mutex_lock(&program->last_meal_lock);
		if (program->philos[i].stopped == 0
			&& (poll_death(&program->philos[i]) == 0)
				&& (get_current_time() - program->philos[i].last_meal
					> program->philos[i].time_to_die))
		{
			pthread_mutex_unlock(&program->last_meal_lock);
			announce_death(&program->philos[i]);
			pthread_mutex_lock(program->philos[i].write_lock);
			printf("%s%zu %d write locked [Monitor]%s\n", philo_color(program->philos[i].id), get_current_time() - program->philos[i].start_time, program->philos[i].id, C_RESET);//
			pthread_mutex_lock(program->philos[i].death_lock);
			printf("%s%zu %d died at %zu bc last meal time %zu is more than limit %zu%s\n", philo_color(program->philos[i].id), get_current_time() - program->philos[i].start_time, program->philos[i].id, program->philos[i].time_died, program->philos[i].last_meal - program->philos[i].start_time, program->philos[i].time_to_die, C_RESET);//
			printf("%zu %d died\n", get_current_time() - program->philos[i].start_time, program->philos[i].id);
			pthread_mutex_unlock(program->philos[i].death_lock);
			printf("%s%zu %d write unlocked [Monitor]%s\n", philo_color(program->philos[i].id), get_current_time() - program->philos[i].start_time, program->philos[i].id, C_RESET);//
			pthread_mutex_unlock(program->philos[i].write_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&program->last_meal_lock);
		pthread_mutex_lock(&program->last_meal_lock);
		if (program->philos[i].stopped == 1)
		{
			ctr++;
			program->philos[i].stopped = -1;
			if (ctr > 0 && ctr == program->num_of_philos)
			{
				pthread_mutex_unlock(&program->last_meal_lock);
				announce_death(&program->philos[i]);
				pthread_mutex_lock(program->philos[i].write_lock);
				printf("%s%zu %d write locked [Monitor]%s\n", philo_color(program->philos[i].id), get_current_time() - program->philos[i].start_time, program->philos[i].id, C_RESET);//
				printf("All has eaten, exiting...\n");//
				printf("%s%zu %d write unlocked [Monitor]%s\n", philo_color(program->philos[i].id), get_current_time() - program->philos[i].start_time, program->philos[i].id, C_RESET);//
				pthread_mutex_unlock(program->philos[i].write_lock);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&program->last_meal_lock);
		i++;
		if (i == program->num_of_philos)
			i = 0;
	}
	return (NULL);
}
