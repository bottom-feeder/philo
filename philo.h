#ifndef PHILO_H
#define PHILO_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define PHILOS_MAX 200
#define C_RESET "\033[0m"

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				stopped;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	size_t			time_died;
	int				num_times_to_eat;
	int				num_of_philos;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*last_meal_lock;
}					t_philo;

typedef struct s_program
{
	int				num_of_philos;
	int				death_flag;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	last_meal_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	forks[PHILOS_MAX];
	t_philo			philos[PHILOS_MAX];
}					t_program;

void	announce_death(t_philo *philo);
void	destroyer(char *string, t_program* program);
int		eat_routine(t_philo *philo);
int		error_handler(int ac, char** av);
int		fork_pickup_routine(t_philo *philo);
int		ft_isdigit(int c);
int		ft_safe_atoi(char *str);
int		ft_strlen(char *str);
int		ft_usleep(size_t ms, t_philo *philo);
size_t	get_current_time(void);
void	init_l_r_forks(t_program *program);
void	init_fork_mutexes(t_program *program);
void	init_philos(int ac, char **av, t_philo *philos, t_program *program);
int		init_threads(t_program *program);
int		poll_death(t_philo *philo);
void	joiner(t_program* program, int started);
void	*monitor(void *arg);
void	*philo_routine(void *arg);
int		sleep_routine(t_philo *philo);
void	think_routine(t_philo *philo);
const char	*philo_color(int id);
#endif