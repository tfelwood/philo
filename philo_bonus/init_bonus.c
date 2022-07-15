/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 11:19:20 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 12:13:18 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "philo_bonus.h"

static void	ft_sem_destroy(sem_t *sem, const char *name)
{
	if (sem != SEM_FAILED && name)
	{
		sem_close(sem);
		sem_unlink(name);
	}
}

void	ft_sem_all_destroy(t_philo *ph)
{
	ft_sem_destroy(ph->info.print_sem, "/print");
	ft_sem_destroy(ph->info.satiety_sem, "/satiety");
	ft_sem_destroy(ph->info.forks_sem, "/forks");
	ft_sem_destroy(ph->info.synchro_1_sem, "/synchro_1");
	ft_sem_destroy(ph->info.synchro_2_sem, "/synchro_2");
	ft_sem_destroy(ph->philo_sem, ph->eat_time_sem_name);
}

static int	ft_args_init(t_info *info, int argc, char **argv)
{
	info->num_of_feed = 0;
	if (ft_strtoi(*argv++, &info->num) || info->num <= 0
		|| ft_strtoi(*argv++, &info->die_time) || info->die_time <= 0
		|| ft_strtoi(*argv++, &info->eat_time) || info->eat_time <= 0
		|| ft_strtoi(*argv++, &info->sleep_time) || info->sleep_time <= 0
		|| (argc == 5 && (ft_strtoi(*argv, &info->num_of_feed)
				|| info->num_of_feed <= 0)))
		return (BAD_ARGS);
	return (NO_ERR);
}

static int	ft_sem_init(t_philo *ph)
{
	sem_unlink("/print");
	sem_unlink("/satiety");
	sem_unlink("/forks");
	sem_unlink("/synchro_1");
	sem_unlink("/synchro_2");
	ph->philo_sem = SEM_FAILED;
	ph->info.print_sem = sem_open("/print", O_CREAT, 0644, 1);
	ph->info.satiety_sem = sem_open("/satiety", O_CREAT, 0644, 0);
	ph->info.forks_sem = sem_open("/forks", O_CREAT, 0644, ph->info.num);
	ph->info.synchro_1_sem = sem_open("/synchro_1", O_CREAT, 0644, 0);
	ph->info.synchro_2_sem = sem_open("/synchro_2", O_CREAT, 0644, 0);
	if (ph->info.print_sem == SEM_FAILED
		||ph->info.satiety_sem == SEM_FAILED
		||ph->info.forks_sem == SEM_FAILED
		||ph->info.synchro_1_sem == SEM_FAILED
		||ph->info.synchro_2_sem == SEM_FAILED)
		return (SEM_ERROR);
	return (NO_ERR);
}

int	ft_init(t_philo **ph, int argc, char **argv)
{
	*ph = malloc (sizeof(t_philo));
	if (!*ph)
		return (MALLOC_ERROR);
	memset(*ph, 0, sizeof(t_philo));
	if (ft_args_init(&(*ph)->info, argc, argv) != NO_ERR)
	{
		ft_destroy(ph);
		return (BAD_ARGS);
	}
	(*ph)->info.philo_pids = malloc(sizeof(pid_t) * (*ph)->info.num);
	if (!(*ph)->info.philo_pids)
	{
		ft_destroy(ph);
		return (MALLOC_ERROR);
	}
	if (ft_sem_init(*ph) != NO_ERR)
	{
		ft_destroy(ph);
		return (SEM_ERROR);
	}
	(*ph)->info.start_prog = ft_time();
	return (NO_ERR);
}
