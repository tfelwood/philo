/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 23:10:32 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/11 22:41:00 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int ft_error(t_error err)
{
	const char *errors[ERR_NUM] =
			{"Wrong number of arguments", "Wrong value of arguments",
			 "Malloc error", "Pthread error", "Fork error", "Semaphore error"};

	if (err > NO_ERR && err <= ERR_NUM)
		printf("%s\n", errors[err - 1]);
	return (err);
}

void ft_kill(t_info *info, int num)
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
			info->philo_pids[num] = -1;// подумать, делать ли запись, нужно оградить
		}
	}
}

int ft_fork_process(int (*foo)(t_philo *), t_philo *ph)
{
	pid_t	pid;
	int 	return_val;

	pid = fork();
	if (pid == 0)
	{
		return_val = foo(ph);
		ft_destroy(&ph);
		exit(return_val);
	}
	return (pid);
}

int ft_have_fed(t_philo *ph)
{
	int i;

	i = 0;
	while (i < ph->info.num)
	{
		sem_wait(ph->info.satiety_sem);
		++i;
	}
	return (NO_ERR);
}

int ft_synchro(t_philo *ph)
{
	int i;

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

int ft_fork(t_philo *ph)
{
	ph->id = 0;

//	ph->info.synchro_prcs = ft_fork_process(ft_synchro, ph);
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

void	ft_wait(t_info *info)
{
	pid_t	pid;
	int 	i;

	i = 0;
	pid = waitpid(-1, 0, 0);
	if (pid > 0)
	{
		if (pid == info->synchro_prcs)
			pid = waitpid(-1, 0, 0);
		while (i < info->num)
		{
			if (info->philo_pids[i] != pid && info->philo_pids[i] != info->synchro_prcs)
				kill(info->philo_pids[i], SIGTERM);//todoпеределать под килл олл!
			++i;
		}
	}
	if (info->sat_control_prcs > 0 )
		kill(info->sat_control_prcs, SIGTERM);
	if (info->synchro_prcs > 0 )
		kill(info->synchro_prcs, SIGTERM);
}

int main(int argc, char **argv)
{
	t_philo	*ph;

	if (argc == 5 || argc == 6)
	{
		if (ft_error(ft_init(&ph, argc - 1, argv + 1)) ||
			ft_error(ft_fork(ph)))
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
