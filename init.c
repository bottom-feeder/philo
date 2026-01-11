/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:52:25 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 21:09:31 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_fork_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_phs)
	{
		pthread_mutex_init(&(program->forks)[i], NULL);
		i++;
	}
}

void	init_inputs(int ac, char **av, t_program *program, int i)
{
	program->phs[i].num_of_phs = program->num_of_phs;
	program->phs[i].t2d = ft_safe_atoi(av[2]);
	program->phs[i].t2e = ft_safe_atoi(av[3]);
	program->phs[i].t2s = ft_safe_atoi(av[4]);
	if (ac == 6)
		program->phs[i].num_times_to_eat = ft_safe_atoi(av[5]);
	else
		program->phs[i].num_times_to_eat = 0;
}

void	init_philos(int ac, char **av, t_philo *phs, t_program *program)
{
	int	i;

	i = 0;
	program->start_time = gct();
	while (i <= ft_safe_atoi(av[1]))
	{
		phs[i].id = i + 1;
		init_inputs(ac, av, program, i);
		phs[i].stop = 0;
		phs[i].meals_eaten = 0;
		phs[i].start_time = program->start_time;
		phs[i].l_meal = program->start_time;
		phs[i].wlck = &program->wlck;
		phs[i].dlck = &program->dlck;
		phs[i].l_meal_lock = &program->l_meal_lock;
		phs[i].dead = &program->death_flag;
		phs[i].time_died = 0;
		i++;
	}
}

void	init_l_rfs(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_phs)
	{
		if (program->phs[i].id == 1)
			program->phs[i].rf = &program->forks[program->num_of_phs - 1];
		else
			program->phs[i].rf = &program->forks[program->phs[i].id - 2];
		program->phs[i].lf = &program->forks[program->phs[i].id - 1];
		i++;
	}
}

int	init_threads(t_program *pr)
{
	int	i;
	int	started;

	i = 0;
	started = 0;
	while (i < pr->num_of_phs)
	{
		if (pthread_create(&pr->phs[i].thread, NULL, ph_routine, &pr->phs[i]))
		{
			announce_death(&pr->phs[i]);
			joiner(pr, started);
			destroyer("Pthread creation failed", pr);
			return (1);
		}
		started++;
		i++;
	}
	if (pthread_create(&pr->phs[i].thread, NULL, monitor, pr))
	{
		announce_death(&pr->phs[i - 1]);
		joiner(pr, started);
		destroyer("Pthread creation failed", pr);
		return (1);
	}
	return (0);
}
