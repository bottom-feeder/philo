/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:47:50 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/10 18:27:28 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

const char *philo_color(int id)
{
	static char	buf[16];
	int		color;
	color = 16 + (id * 7) % 216;
	snprintf(buf, sizeof(buf), "\033[38;5;%dm", color);
	return (buf);
}

void joiner(t_program* program, int started)
{
//	pthread_mutex_lock(&program->write_lock); //debug
//	printf("Joiner started! Started is %d\n", started); //debug
//	pthread_mutex_unlock(&program->write_lock); //debug
	int	i;

	i = 0;
	while (i < started)
	{
		if (pthread_join(program->philos[i].thread, NULL))
			write(2, "Thread join error!\n", 19);
//		pthread_mutex_lock(program->philos[i].write_lock); //debug
//		printf("%zu Joined thread #%lu\n", get_current_time() - program->philos[i].start_time, (unsigned long)program->philos[i].thread); //debug
//		pthread_mutex_unlock(program->philos[i].write_lock); //debug
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