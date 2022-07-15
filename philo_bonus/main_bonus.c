/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 23:10:32 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 12:23:24 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_error(t_error err)
{
	const char	*errors[ERR_NUM] = {"Wrong number of arguments",
		"Wrong value of arguments", "Malloc error",
		"Pthread error", "Fork error", "Semaphore error"};

	if (err > NO_ERR && err <= ERR_NUM)
		printf("%s\n", errors[err - 1]);
	return (err);
}

void	ft_kill(t_info *info, int num)
{
	if (info->sat_control_prcs > 0)
		kill(info->sat_control_prcs, SIGTERM);
	if (info->synchro_prcs > 0)
		kill(info->synchro_prcs, SIGTERM);
	if (!info->philo_pids)
		return ;
	while (num--)
	{
		if (info->philo_pids[num] != -1)
		{
			kill(info->philo_pids[num], SIGTERM);
			info->philo_pids[num] = -1;
		}
	}
}

void	ft_destroy(t_philo **ph)
{
	if (*ph)
	{
		ft_sem_all_destroy(*ph);
		free((*ph)->info.philo_pids);
		free((*ph)->eat_time_sem_name);
		free(*ph);
		*ph = NULL;
	}
}

static void	ft_wait(t_info *info)
{
	pid_t	pid;
	int		i;

	i = 0;
	pid = waitpid(-1, 0, 0);
	if (pid > 0)
	{
		if (pid == info->synchro_prcs)
			pid = waitpid(-1, 0, 0);
		while (i < info->num)
		{
			if (info->philo_pids[i] != pid
				&& info->philo_pids[i] != info->synchro_prcs)
				kill(info->philo_pids[i], SIGTERM);
			++i;
		}
	}
	if (info->sat_control_prcs > 0)
		kill(info->sat_control_prcs, SIGTERM);
	if (info->synchro_prcs > 0)
		kill(info->synchro_prcs, SIGTERM);
}

int	main(int argc, char **argv)
{
	t_philo	*ph;

	if (argc == 5 || argc == 6)
	{
		if (ft_error(ft_init(&ph, argc - 1, argv + 1))
			|| ft_error(ft_fork(ph)))
		{
			ft_destroy(&ph);
			return (EXIT_FAILURE);
		}
		ft_wait(&ph->info);
		ft_destroy(&ph);
	}
	else
	{
		ft_error(BAD_NUM_ARG);
		return (EXIT_FAILURE);
	}
}
