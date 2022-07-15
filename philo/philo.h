/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 21:19:48 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 12:44:07 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

# define ERR_NUM 5
# define STATUS_NUM 5
# define DELAY 600
# define WATCHER_DELAY 500

typedef enum e_error
{
	NO_ERR,
	BAD_NUM_ARG,
	BAD_ARGS,
	MALLOC_ERROR,
	PTHREAD_ERROR,
	MUTEX_ERROR
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
	int				num;
	int				die;
	int				eat;
	int				sleep;
	int				num_of_feed;
	long long		start;
	int				exit;
	int				satiety;
	pthread_mutex_t	exit_mtx;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	satiety_mtx;
	pthread_mutex_t	*forks;
}	t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		thrd;
	long long		last_eat;
	t_info			*info;
	int				first;
	int				second;
	int				num_of_feed;
	pthread_mutex_t	mtx;
}	t_philo;

int			ft_strtoi(const char *str, int *number);
long long	ft_time(void);
void		ft_sleep(long long ms);
void		ft_print(t_philo *ph, t_status st, t_info *info);
int			ft_philo_arr_init(t_philo **arr, t_info *info);
void		ft_philo_arr_destroy(t_philo **arr, int num);
int			ft_init(t_info *info, t_philo **arr, int argc, char **argv);
void		ft_info_destroy(t_info *info);
#endif
