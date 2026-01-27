/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:35:30 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/16 01:13:28 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_program	program;

	if (!error_handler(ac, av))
		return (1);
	init_philos(ac, av, program.phs, &program);
	init_fork_mutexes(&program);
	init_l_r_forks(&program);
	if (init_threads(&program) == 0)
	{
		joiner(&program, program.num_of_phs + 1);
		destroyer(NULL, &program);
	}
	return (0);
}
