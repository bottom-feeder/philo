/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikiriush <ikiriush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 21:04:42 by ikiriush          #+#    #+#             */
/*   Updated: 2026/01/11 21:09:09 by ikiriush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

# define PHILOS_MAX 201

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				stop;
	int				meals_eaten;
	size_t			l_meal;
	size_t			t2d;
	size_t			t2e;
	size_t			t2s;
	size_t			start_time;
	size_t			time_died;
	int				num_times_to_eat;
	int				num_of_phs;
	int				*dead;
	pthread_mutex_t	*rf;
	pthread_mutex_t	*lf;
	pthread_mutex_t	*wlck;
	pthread_mutex_t	*dlck;
	pthread_mutex_t	*l_meal_lock;
}					t_philo;

typedef struct s_program
{
	int				num_of_phs;
	int				death_flag;
	size_t			start_time;
	pthread_mutex_t	dlck;
	pthread_mutex_t	l_meal_lock;
	pthread_mutex_t	wlck;
	pthread_mutex_t	forks[PHILOS_MAX];
	t_philo			phs[PHILOS_MAX];
}					t_program;

void	announce_death(t_philo *ph);
void	destroyer(char *string, t_program *program);
int		eat_routine(t_philo *ph);
int		error_handler(int ac, char **av);
int		fork_pickup_routine(t_philo *ph);
int		ft_isdigit(int c);
int		ft_safe_atoi(char *str);
int		ft_strlen(char *str);
int		ft_usleep(size_t ms, t_philo *ph);
size_t	gct(void);
void	init_l_rfs(t_program *program);
void	init_fork_mutexes(t_program *program);
void	init_philos(int ac, char **av, t_philo *phs, t_program *program);
int		init_threads(t_program *pr);
int		poll_death(t_philo *ph);
void	joiner(t_program *program, int started);
void	*monitor(void *arg);
void	*ph_routine(void *arg);
int		sleep_routine(t_philo *ph);
int		take_1st_fork(t_philo *ph, int l_first);
int		take_2nd_fork(t_philo *ph, int l_first);
int		think_routine(t_philo *ph);

#endif