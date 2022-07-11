/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 21:56:16 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/10 18:15:24 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/fcntl.h>

#define ERR_NUM 6
#define STATUS_NUM 5
#define PHILO_TAG	"/philo_"
typedef enum e_error
{
	NO_ERR,
	BAD_NUM_ARG,
	BAD_ARGS,
	MALLOC_ERROR,
	PTHREAD_ERROR,
	FORK_ERROR,
	SEM_ERROR
}	t_error;

typedef enum e_status
{
	THINK,
	HAS_TAKEN_A_FORK,
	EAT,
	SLEEP,
	DIED
}	t_status;

typedef struct s_info
{
	int			num;
	int 		die_time;
	int 		eat_time;
	int			sleep_time;
	int 		num_of_feed;
	long long	start_prog; //init
	sem_t		*print_sem;
	sem_t		*satiety_sem;
	sem_t		*forks_sem;
	sem_t		*synchro_1_sem;
	sem_t		*synchro_2_sem;
	pid_t		*philo_pids; //init
	pid_t		synchro_prcs;
	pid_t		sat_control_prcs;
}	t_info;

typedef struct s_philo
{
	int			id; //init
	long long	eat_time; //init
	int			num_of_feed;
	sem_t		*philo_sem; //init
	char 		*eat_time_sem_name; //init
	pthread_t	watcher; //init
	t_info		info;
}	t_philo;

int	ft_strtoi(const char *str, int *number);
int ft_init(t_philo **ph, int argc, char **argv);
void ft_destroy(t_philo **ph);
void ft_kill(t_info *info, int num);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
int ft_error(t_error err);
long long	ft_time();
void ft_sleep(long long ms);
void ft_print(t_philo *ph, t_status st);
int	ft_philo_life(t_philo *ph);
#endif
