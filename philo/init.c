/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:33:55 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/08 21:19:24 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "philo.h"

static void	ft_mtx_destroy(pthread_mutex_t *arr, int num)
{
	while (num--)
		pthread_mutex_destroy(&arr[num]);
}

static int	ft_mutex_arr_create(pthread_mutex_t **arr, int num)
{
	int	i;

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
	pthread_mutex_destroy(&info->satiety_mtx);
}

static int	ft_info_init(t_info *info, int argc, char **argv)
{
	int	err;

	err = NO_ERR;
	memset(info, 0, sizeof(*info));
	if (ft_strtoi(*argv++, &info->num) || info->num <= 0
		|| ft_strtoi(*argv++, &info->die) || info->die <= 0
		|| ft_strtoi(*argv++, &info->eat) || info->eat <= 0
		|| ft_strtoi(*argv++, &info->sleep) || info->sleep <= 0
		|| (argc == 5 && (ft_strtoi(*argv, &info->num_of_feed)
				|| info->num_of_feed <= 0)))
		return (BAD_ARGS);
	if (pthread_mutex_init(&info->print_mtx, NULL)
		|| pthread_mutex_init(&info->exit_mtx, NULL)
		||pthread_mutex_init(&info->satiety_mtx, NULL))
		err = MUTEX_ERROR;
	if (err == NO_ERR)
		err = ft_mutex_arr_create(&info->forks, info->num);
	if (err != NO_ERR)
		ft_info_destroy(info);
	info->start = ft_time();
	return (err);
}

int	ft_init(t_info *info, t_philo **arr, int argc, char **argv)
{
	t_error	err;

	err = ft_info_init(info, argc, argv);
	if (err == NO_ERR)
		err = ft_philo_arr_init(arr, info);
	if (err != NO_ERR)
	{
		ft_philo_arr_destroy(arr, info->num);
		ft_info_destroy(info);
	}
	return (err);
}
