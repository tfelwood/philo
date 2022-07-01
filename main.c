/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:34:03 by tfelwood          #+#    #+#             */
/*   Updated: 2022/06/17 15:06:31 by tfelwood         ###   ########.fr       */
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

static void	ft_watch_philo(t_philo *arr)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < arr->info->num)
		{
			if (ft_is_died(&arr[i]))
			{
				ft_print(arr[i].id, DIED, arr[i].info);
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

	if (argc == 5 || argc == 6)
	{
		if (ft_error(ft_init(&info, &arr, argc - 1, argv + 1)))
			return (EXIT_FAILURE);
		else
		{
			ft_watch_philo(arr);
			ft_destroy(&arr, &info);
		}
	}
	else
		ft_error(BAD_NUM_ARG);
}
