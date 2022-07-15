/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 15:47:48 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/15 12:14:09 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_is_died(t_philo *ph)
{
	long long	eat_time;

	sem_wait(ph->philo_sem);
	eat_time = ph->eat_time;
	sem_post(ph->philo_sem);
	return (ft_time() - eat_time >= ph->info.die_time);
}

static void	*ft_watch(void *ptr)
{
	t_philo		*ph;

	ph = (t_philo *)ptr;
	while (!ft_is_died(ph))
		usleep(1000);
	ft_print(ph, DIED);
	exit(DIED);
}

static int	ft_philo_init(t_philo *ph)
{
	char	*tmp;

	tmp = ft_itoa(ph->id);
	ph->eat_time_sem_name = ft_strjoin(PHILO_TAG, tmp);
	free(tmp);
	if (!ph->eat_time_sem_name)
		return (MALLOC_ERROR);
	sem_unlink(ph->eat_time_sem_name);
	ph->philo_sem = sem_open(ph->eat_time_sem_name, O_CREAT, 0644, 1);
	if (ph->philo_sem == SEM_FAILED)
		return (SEM_ERROR);
	ph->eat_time = ph->info.start_prog;
	if (pthread_create(&ph->watcher, NULL, ft_watch, ph))
		return (PTHREAD_ERROR);
	pthread_detach(ph->watcher);
	return (NO_ERR);
}

static int	ft_philo_prepare(t_philo *ph)
{
	enum e_error	err;

	sem_post(ph->info.synchro_1_sem);
	sem_wait(ph->info.synchro_2_sem);
	err = ft_philo_init(ph);
	if (err != NO_ERR)
	{
		sem_wait(ph->info.print_sem);
		ft_error(err);
		ft_destroy(&ph);
		return (EXIT_FAILURE);
	}
	if (ph->info.num > 3 && ph->id % 2 == 0)
		usleep((ph->info.eat_time) * 1000 + 600);
	if (ph->info.num <= 3)
		usleep((ph->info.eat_time) * 1000 + 600);
	return (NO_ERR);
}

int	ft_philo_life(t_philo *ph)
{
	if (ft_philo_prepare(ph) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (1)
	{
		sem_wait(ph->info.forks_sem);
		ft_print(ph, HAS_TAKEN_A_FORK);
		sem_wait(ph->info.forks_sem);
		ft_print(ph, HAS_TAKEN_A_FORK);
		sem_wait(ph->philo_sem);
		ph->eat_time = ft_time();
		sem_post(ph->philo_sem);
		ft_print(ph, EAT);
		ft_sleep(ph->info.eat_time);
		sem_post(ph->info.forks_sem);
		sem_post(ph->info.forks_sem);
		++ph->num_of_feed;
		if (ph->num_of_feed == ph->info.num_of_feed)
			sem_post(ph->info.satiety_sem);
		ft_print(ph, SLEEP);
		ft_sleep(ph->info.sleep_time);
		ft_print(ph, THINK);
		usleep(500);
	}
}

/* 112 была закоменчнеа*/