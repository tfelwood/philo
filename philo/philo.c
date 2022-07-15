/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:28:24 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 11:53:49 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_eat(t_philo *ph)
{
	pthread_mutex_lock(&ph->info->forks[ph->first]);
	ft_print(ph, HAS_TAKEN_A_FORK, ph->info);
	if (ph->info->num == 1)
	{
		ft_sleep(ph->info->sleep);
		pthread_mutex_unlock(&ph->info->forks[ph->first]);
		return (1);
	}
	pthread_mutex_lock(&ph->info->forks[ph->second]);
	ft_print(ph, HAS_TAKEN_A_FORK, ph->info);
	pthread_mutex_lock(&ph->mtx);
	ph->last_eat = ft_time();
	pthread_mutex_unlock(&ph->mtx);
	ft_print(ph, EAT, ph->info);
	ft_sleep(ph->info->eat);
	++ph->num_of_feed;
	pthread_mutex_unlock(&ph->info->forks[ph->second]);
	pthread_mutex_unlock(&ph->info->forks[ph->first]);
	if (ph->num_of_feed == ph->info->num_of_feed)
	{
		pthread_mutex_lock(&ph->info->satiety_mtx);
		++ph->info->satiety;
		pthread_mutex_unlock(&ph->info->satiety_mtx);
	}
	return (0);
}

void	*ft_philo_live(void *data)
{
	t_philo	*ph;

	ph = (t_philo *)data;
	pthread_mutex_lock(&ph->info->exit_mtx);
	pthread_mutex_unlock(&ph->info->exit_mtx);
	if (ph->info->num > 3 && ph->id % 2 == 0)
		usleep((ph->info->eat) * 1000 + DELAY);
	while (1)
	{
		if (ft_eat(ph))
			break ;
		ft_print(ph, SLEEP, ph->info);
		ft_sleep(ph->info->sleep);
		pthread_mutex_lock(&ph->info->exit_mtx);
		if (ph->info->exit)
			break ;
		pthread_mutex_unlock(&ph->info->exit_mtx);
		ft_print(ph, THINK, ph->info);
		if (ph->info->num % 2 == 1)
			usleep(DELAY);
	}
	pthread_mutex_unlock(&ph->info->exit_mtx);
	return (NULL);
}

static int	ft_philo_init(int id, t_philo *philo, t_info *info)
{
	philo->id = id;
	philo->num_of_feed = 0;
	philo->last_eat = info->start;
	philo->info = info;
	if (id == 1)
	{
		philo->first = 0;
		philo->second = info->num - 1;
	}
	else
	{
		philo->first = id - 2;
		philo->second = id - 1;
	}
	if (pthread_mutex_init(&philo->mtx, NULL))
		return (MUTEX_ERROR);
	if (pthread_create(&philo->thrd, NULL, ft_philo_live, (void *)philo))
	{
		pthread_mutex_destroy(&philo->mtx);
		return (PTHREAD_ERROR);
	}
	return (NO_ERR);
}

void	ft_philo_arr_destroy(t_philo **arr, int num)
{
	if (!*arr)
		return ;
	pthread_mutex_lock(&(*arr)->info->exit_mtx);
	(*arr)->info->exit = 1;
	pthread_mutex_unlock(&(*arr)->info->exit_mtx);
	while (num--)
	{
		pthread_join((*arr + num)->thrd, NULL);
		pthread_mutex_destroy(&(*arr + num)->mtx);
	}
	free(*arr);
	*arr = NULL;
}

int	ft_philo_arr_init(t_philo **arr, t_info *info)
{
	enum e_error	err;
	int				i;

	err = NO_ERR;
	i = 0;
	*arr = (t_philo *)malloc(info->num * sizeof(t_philo));
	if (!*arr)
		return (MALLOC_ERROR);
	pthread_mutex_lock(&info->exit_mtx);
	while (i < info->num)
	{
		err = ft_philo_init(i + 1, *arr + i, info);
		if (err != NO_ERR)
		{
			info->exit = 1;
			pthread_mutex_unlock(&info->exit_mtx);
			ft_philo_arr_destroy(arr, i);
			break ;
		}
		++i;
	}
	pthread_mutex_unlock(&info->exit_mtx);
	return (err);
}
