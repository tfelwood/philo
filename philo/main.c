/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:34:03 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/09 02:07:52 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_error(t_error err)
{
	const char *errors[ERR_NUM] =
			{"Wrong number of arguments", "Wrong value of arguments",
			 "Malloc error", "Pthread error", "Mutex error"};

	if (err > NO_ERR && err <= ERR_NUM)
		printf("%s\n", errors[err - 1]);
	return (err);
}

static int ft_is_died(t_philo *ph)
{
	long long time;

	pthread_mutex_lock(&ph->mtx);
	time = ph->last_eat;
	pthread_mutex_unlock(&ph->mtx);
	return (ft_time() - time >= ph->info->die);
}

static int ft_have_fed(t_info *info)
{
	int	tmp_sat;

	pthread_mutex_lock(&info->satiety_mtx);
	tmp_sat = info->satiety;
	pthread_mutex_unlock(&info->satiety_mtx);
	return (tmp_sat >= info->num);
}

static void	ft_watch_philo(t_philo *arr)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < arr->info->num)
		{
			if (ft_have_fed(arr->info))
				return ;
			if (ft_is_died(&arr[i]))
			{
				pthread_mutex_lock(&arr->info->exit_mtx);
				arr->info->exit = 1;
				pthread_mutex_unlock(&arr->info->exit_mtx);
				ft_print(&arr[i], DIED, arr[i].info);
				return ;
			}
			++i;
		}
		usleep(100);
	}
}

int main(int argc, char **argv)
{
	t_info	info;
	t_philo *arr;

	arr = NULL;
	if (argc == 5 || argc == 6)
	{
		if (ft_error(ft_init(&info, &arr, argc - 1, argv + 1)))
			return (EXIT_FAILURE);
		else
		{
			ft_watch_philo(arr);
			ft_destroy(&info, &arr);
		}
	}
	else
		ft_error(BAD_NUM_ARG);
}
