/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:52:25 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/08 02:28:34 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_fork_mutexes(t_program* program)
{
	int i;

	i = 0;
	while (i < program->num_of_philos)
	{
		pthread_mutex_init(&(program->forks)[i], NULL);
		i++;
	}
}

void	init_inputs(int ac, char** av, t_program *program, int i)
{
	program->philos[i].num_of_philos = program->num_of_philos;
	program->philos[i].time_to_die = ft_safe_atoi(av[2]);
	program->philos[i].time_to_eat = ft_safe_atoi(av[3]);
	program->philos[i].time_to_sleep = ft_safe_atoi(av[4]);
	if (ac == 6)
		program->philos[i].num_times_to_eat = ft_safe_atoi(av[5]);
	else
		program->philos[i].num_times_to_eat = 0;
}

void	init_philos(int ac, char** av, t_philo *philos, t_program *program)
{
	int	i;
	
	i = 0;
	while (i <= ft_safe_atoi(av[1]))
	{
		philos[i].id = i + 1;
		init_inputs(ac, av, program, i);
		philos[i].stopped = 0;
		philos[i].meals_eaten = 0;
		philos[i].start_time = get_current_time();
		philos[i].last_meal = get_current_time();
		philos[i].write_lock = &program->write_lock;
		philos[i].death_lock = &program->death_lock;
		philos[i].last_meal_lock = &program->last_meal_lock;
		philos[i].dead = &program->death_flag;
		philos[i].time_died = 0;
		i++;
	}
}

void	init_l_r_forks(t_program *program)
{
	int	i;
	
	i = 0;
	while (i < program->num_of_philos)
	{
		if (program->philos[i].id == 1)
			program->philos[i].r_fork = &program->forks[program->num_of_philos - 1];
		else
			program->philos[i].r_fork = &program->forks[program->philos[i].id - 2];
		program->philos[i].l_fork = &program->forks[program->philos[i].id - 1];
		i++;
	}
}

int	init_threads(t_program *program)
{
	int	i;
	int	started;

	i = 0;
	started = 0;
	while (i < program->num_of_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, philo_routine, &program->philos[i]))
		{
			announce_death(&program->philos[i]);
			joiner(program, started);
			destroyer("Pthread creation failed", program);
			return (1);
		}
		//printf("Thread ID = %lu has launched\n", (unsigned long)program->philos[i].thread);
		started++;
		//printf("started value = %d\n", started);
		i++;
		//printf("i = %d\n", i);
	}
	if (pthread_create(&program->philos[i].thread, NULL, monitor, program))
	{
		announce_death(&program->philos[i - 1]);
		joiner(program, started);
		destroyer("Pthread creation failed", program);
		return (1);
	}
	//printf("Monitor Thread ID = %lu has launched\n", (unsigned long)program->philos[i].thread);
	//printf("started value = %d\n", started);
	return (0);
}
