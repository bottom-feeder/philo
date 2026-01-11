/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:35:30 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 22:05:56 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_program	program;

	if (!error_handler(ac, av))
		return (1);
	program.death_flag = 0;
	pthread_mutex_init(&program.dlck, NULL);
	pthread_mutex_init(&program.l_meal_lock, NULL);
	pthread_mutex_init(&program.wlck, NULL);
	program.num_of_phs = ft_safe_atoi(av[1]);
	init_philos(ac, av, program.phs, &program);
	init_fork_mutexes(&program);
	init_l_rfs(&program);
	if (init_threads(&program) == 0)
	{
		joiner(&program, program.num_of_phs + 1);
		destroyer(NULL, &program);
	}
	return (0);
}
