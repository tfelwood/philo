/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:33:55 by tfelwood          #+#    #+#             */
/*   Updated: 2022/06/17 13:52:58 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_mtx_destroy(pthread_mutex_t *arr, int num)
{
	while (num--)
		pthread_mutex_destroy(&arr[num]);
}

int	ft_mutex_arr_create(pthread_mutex_t **arr, int num)
{
	int i;

	i = 0;
	*arr = (pthread_mutex_t *)malloc(num * sizeof(pthread_mutex_t));
	if (!*arr)
		return (MALLOC_ERROR);
	while (i < num)
	{
		if (pthread_mutex_init(*arr + i, NULL))
		{
			ft_mtx_destroy(*arr, i);
			free(*arr);
			*arr = NULL;
			return (MUTEX_ERROR);
		}
		++i;
	}
	return (NO_ERR);
}

void	ft_info_destroy(t_info *info)
{
	if (info->forks)
	{
		ft_mtx_destroy(info->forks, info->num);
		free(info->forks);
		info->forks = NULL;
	}
	pthread_mutex_destroy(&info->print_mtx);
	pthread_mutex_destroy(&info->exit_mtx);
	pthread_mutex_destroy(&info->num_of_feed_mtx);
}

int	ft_info_init(t_info *info, int argc, char **argv)
{
	t_error	err;

	info->exit = 0;
	if (ft_strtoi(*argv++, &info->num) || info->num <= 0
		|| ft_strtoi(*argv++, &info->die) || info->die <= 0
		|| ft_strtoi(*argv++, &info->eat) || info->eat <= 0
		|| ft_strtoi(*argv++, &info->sleep) || info->sleep <= 0
		|| (argc == 5 && (ft_strtoi(*argv, &info->feed) || info->feed <= 0)))
		return (BAD_ARGS);
	if (pthread_mutex_init(&info->print_mtx, NULL))
		return (MUTEX_ERROR);
	if (pthread_mutex_init(&info->exit_mtx, NULL))
	{
		pthread_mutex_destroy(&info->print_mtx);
		return (MUTEX_ERROR);
	}
	err = ft_mutex_arr_create(&info->forks, info->num);
	if (err != NO_ERR)
	{
		pthread_mutex_destroy(&info->print_mtx);
		pthread_mutex_destroy(&info->exit_mtx);
	}
	info->start = ft_time();
	return (err);
}

int ft_destroy(t_info *info, t_philo **arr)
{
	if (*arr)
		ft_philo_arr_destroy(arr, info->num);
	ft_info_destroy(info);
	/*if (info->forks)
	{
		ft_mtx_destroy(info->forks, info->num);
		free(info->forks);
		info->forks = NULL;
	}
	pthread_mutex_destroy(&info->print_mtx);
	pthread_mutex_destroy(&info->exit_mtx);*/
	return (0);
}

int	ft_init(t_info *info, t_philo **arr, int argc, char **argv)
{
	t_error err;

	err = ft_info_init(info, argc, argv);
	if (err == NO_ERR)
		err = ft_philo_arr_init(arr, info);
	if (err != NO_ERR)
		ft_destroy(info, arr);
	return (err);
}
