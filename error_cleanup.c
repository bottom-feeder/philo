/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:47:50 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/08 02:28:51 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

const char *philo_color(int id)
{
	static char	buf[16];
	int		color;
	color = 16 + (id * 7) % 216; // spread IDs across cube
	snprintf(buf, sizeof(buf), "\033[38;5;%dm", color);
	return (buf);
}

void joiner(t_program* program, int started)
{
	//printf("Joiner started! Started is %d\n", started);
	int	i;

	i = 0;
	while (i < started)
	{
		if (pthread_join(program->philos[i].thread, NULL))
			write(2, "Thread join error!\n", 19);
		//printf("Joined thread #%lu\n", (unsigned long)program->philos[i].thread);
		i++;
	}
}

int error_handler(int ac, char** av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		return (write(2, "Invalid number of arguments!\n", 29), 0);
	i = 1;
	while(av && av[i])
	{
		j = 0;
		if (av[i][0] == '0' && i != 5)
			return (write(2, "Null argument!\n", 15), 0);
		while(av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (write(2, "Invalid arguments!\n", 19), 0);
			j++;
		}
		i++;
	}
	if (ft_safe_atoi(av[1]) > 200)
		return (write(2, "Max no. exceeded!\n", 18), 0);
	return (1);
}

void destroyer(char *string, t_program* program)
{
	int	i;
	int	n;

	if (string)
		write(2, string, ft_strlen(string));
	pthread_mutex_destroy(&program->death_lock);
	pthread_mutex_destroy(&program->last_meal_lock);
	pthread_mutex_destroy(&program->write_lock);
	i = 0;
	n = program->num_of_philos;
	while (i < n)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
}