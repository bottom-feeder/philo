/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:35:30 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/07 03:18:24 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char** av)
{
	t_program	program;
	
	if (!error_handler(ac, av))
		return (1);
	program.num_of_philos = ft_safe_atoi(av[1]);
	init_philos(ac, av, program.philos, &program);
	init_fork_mutexes(&program);
	init_l_r_forks(&program);
	if (init_threads(&program) == 0)
	{
		joiner(&program, program.num_of_philos + 1);
		destroyer(NULL, &program);
	}
	return (0);
}
