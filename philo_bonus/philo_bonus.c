/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 15:47:48 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/11 22:48:45 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int ft_is_died(t_philo *ph)
{
	long long time;
	long long eat_time;

	time = ft_time();
	sem_wait(ph->philo_sem);
	eat_time = ph->eat_time;
	sem_post(ph->philo_sem);
	return (time - eat_time >= ph->info.die_time);
}

void *ft_watch(void *ptr)
{
	t_philo		*ph;
	//long long	start;

	//start = 0;
	ph = (t_philo *)ptr;
//	while (!start)
//	{
//		sem_wait(ph->philo_sem);
//		start = ph->eat_time;
//		sem_post(ph->philo_sem);
//		usleep(2000);
//	}
	while (!ft_is_died(ph))
		usleep(500);
	ft_print(ph, DIED);
	exit(DIED);
}

/*int	ft_philo_init(t_philo *ph)
{
	char	*tmp;

	ph->eat_time = 0;
	tmp = ft_itoa(ph->id);
	ph->eat_time_sem_name = ft_strjoin(PHILO_TAG, tmp);
	free(tmp);
	if (!ph->eat_time_sem_name)
		return (MALLOC_ERROR);
	sem_unlink(ph->eat_time_sem_name);
	ph->philo_sem = sem_open(ph->eat_time_sem_name, O_CREAT, 0644, 1);
	if (ph->philo_sem == SEM_FAILED)
		return (SEM_ERROR);
	if (pthread_create(&ph->watcher, NULL, ft_watch, ph))
		return (PTHREAD_ERROR);
	pthread_detach(ph->watcher); //подумать над джойном
	return (NO_ERR);
}*/

int	ft_philo_init(t_philo *ph)
{
	char	*tmp;

	ph->eat_time = 0;
	tmp = ft_itoa(ph->id);
	ph->eat_time_sem_name = ft_strjoin(PHILO_TAG, tmp);
	free(tmp);
	if (!ph->eat_time_sem_name)
		return (MALLOC_ERROR);
	sem_unlink(ph->eat_time_sem_name);
	ph->philo_sem = sem_open(ph->eat_time_sem_name, O_CREAT, 0644, 1);
	if (ph->philo_sem == SEM_FAILED)
		return (SEM_ERROR);
	ph->eat_time = ft_time();// no th -> no sem
	if (pthread_create(&ph->watcher, NULL, ft_watch, ph))
		return (PTHREAD_ERROR);
	pthread_detach(ph->watcher); //подумать над джойном
	return (NO_ERR);
}

int	ft_philo_life(t_philo *ph)
{
//	sem_post(ph->info.synchro_1_sem);
//	sem_wait(ph->info.synchro_2_sem);
	if (ft_error(ft_philo_init(ph)))
	{
		ft_destroy(&ph);
		return (EXIT_FAILURE);
	}
//	sem_post(ph->info.synchro_1_sem);
//	sem_wait(ph->info.synchro_2_sem);
//	ft_print(ph, THINK);
//	sem_wait(ph->philo_sem);
//	ph->eat_time = ft_time();
//	sem_post(ph->philo_sem);
//	if (ph->id > ph->info.num / 2)
//		usleep(300);
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
		usleep(100);
	}
}
