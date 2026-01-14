/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 23:16:13 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 22:10:37 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;
	int	ctr;

	i = 0;
	ctr = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		ctr++;
		i++;
	}
	return (ctr);
}

size_t	gct(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 21);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t ms, t_philo *ph)
{
	size_t	start;

	start = gct();
	while (gct() - start < ms)
	{
		if (poll_death(ph))
			return (1);
		usleep(500);
	}
	return (0);
}

int	ft_isdigit(int c)
{
	unsigned char	cu;

	cu = (unsigned char)c;
	if (c < 0 || c > 255)
		return (0);
	if (c == EOF)
		return (0);
	if (cu >= '0' && cu <= '9')
		return (1);
	else
		return (0);
}

int	ft_safe_atoi(char *str)
{
	int		i;
	long	res;

	i = 0;
	res = 0;
	if (!str || !str[0])
		return (-1);
	if (!ft_isdigit(str[i]))
		return (-1);
	while (ft_isdigit(str[i++]))
	{
		if (res > ((2147483647 - (str[i - 1] - '0')) / 10))
			return (-1);
		res = res * 10 + (str[i - 1] - '0');
	}
	if (str[i - 1] != '\0')
		return (-1);
	return (res);
}
