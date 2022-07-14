/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 11:49:17 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/08 23:52:27 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "philo.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_strtoi(const char *str, int *number)
{
	char				is_neg;
	unsigned long long	num;

	is_neg = 1;
	num = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			is_neg = -1;
		++str;
	}
	if (!ft_isdigit(*str))
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		if ((num > (unsigned int) -1 / 2 + 1 && is_neg == -1)
			|| (num > (unsigned int) -1 / 2 && is_neg == 1))
			return (-1);
		++str;
	}
	if (*str)
		return (-1);
	*number = (int)num * is_neg;
	return (0);
}

long long	ft_time(void)
{
	struct timeval	cur_time;

	gettimeofday(&cur_time, NULL);
	return (cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000);
}

void	ft_sleep(long long ms)
{
	long long	time;

	time = ft_time();
	usleep(ms * 950);
	while (ft_time() < time + ms)
		usleep(ms * 2);
}

void	ft_print(t_philo *ph, t_status st, t_info *info)
{
	const char	*msgs[STATUS_NUM] = {"is thinking", "has taken a fork",
	"is eating", "is sleeping", "died"};

	pthread_mutex_lock(&info->print_mtx);
	pthread_mutex_lock(&info->exit_mtx);
	if (info->exit && st != DIED)
	{
		pthread_mutex_unlock(&info->exit_mtx);
		pthread_mutex_unlock(&info->print_mtx);
		return ;
	}
	pthread_mutex_unlock(&info->exit_mtx);
	printf("%lld %d %s\n", ft_time() - info->start, ph->id, msgs[st]);
	pthread_mutex_unlock(&info->print_mtx);
}
