/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 11:30:10 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 11:49:37 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_have_fed(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph->info.num)
	{
		sem_wait(ph->info.satiety_sem);
		++i;
	}
	return (NO_ERR);
}

static int	ft_synchro(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph->info.num)
	{
		sem_wait(ph->info.synchro_1_sem);
		++i;
	}
	while (i--)
		sem_post(ph->info.synchro_2_sem);
	return (NO_ERR);
}

static int	ft_fork_process(int (*foo)(t_philo *), t_philo *ph)
{
	pid_t	pid;
	int		return_val;

	pid = fork();
	if (pid == 0)
	{
		return_val = foo(ph);
		ft_destroy(&ph);
		exit(return_val);
	}
	return (pid);
}

int	ft_fork(t_philo *ph)
{
	ph->id = 0;
	ph->info.synchro_prcs = ft_fork_process(ft_synchro, ph);
	if (ph->info.num_of_feed != 0)
		ph->info.sat_control_prcs = ft_fork_process(ft_have_fed, ph);
	if (ph->info.synchro_prcs == -1 || ph->info.sat_control_prcs == -1)
	{
		ft_kill(&ph->info, 0);
		return (FORK_ERROR);
	}
	while (ph->id < ph->info.num)
	{
		ph->id++;
		ph->info.philo_pids[ph->id - 1] = ft_fork_process(ft_philo_life, ph);
		if (ph->info.philo_pids[ph->id - 1] == -1)
		{
			ft_kill(&ph->info, ph->id - 1);
			return (FORK_ERROR);
		}
	}
	return (NO_ERR);
}
