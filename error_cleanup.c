/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:47:50 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/16 01:10:11 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	joiner(t_program *program, int started)
{
	int	i;

	i = 0;
	while (i < started)
	{
		if (pthread_join(program->phs[i].thread, NULL))
			write(2, "Thread join error!\n", 19);
		i++;
	}
}

int	error_handler(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		return (write(2, "Invalid number of arguments!\n", 29), 0);
	i = 1;
	while (av && av[i])
	{
		j = 0;
		if (av[i][0] == '0' && i != 5)
			return (write(2, "Null argument!\n", 15), 0);
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]) || ft_safe_atoi(av[i]) == -1)
				return (write(2, "Invalid arguments!\n", 19), 0);
			j++;
		}
		i++;
	}
	if (ft_safe_atoi(av[1]) >= PHILOS_MAX)
		return (write(2, "Max no. exceeded!\n", 18), 0);
	return (1);
}

void	destroyer(char *string, t_program *program)
{
	int	i;
	int	n;

	if (string)
		write(2, string, ft_strlen(string));
	pthread_mutex_destroy(&program->dlck);
	pthread_mutex_destroy(&program->mlck);
	pthread_mutex_destroy(&program->wlck);
	i = 0;
	n = program->num_of_phs;
	while (i < n)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
}
