/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 15:54:23 by tfelwood          #+#    #+#             */
/*   Updated: 2022/07/10 15:54:23 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ft_num_length(long long num)
{
	int	size;

	size = 1;
	while (num / 10 != 0)
	{
		num /= 10;
		++size;
	}
	return (size);
}

static void	ft_put_digit(long long n, int idx, char *str)
{
	if (n / 10)
		ft_put_digit (n / 10, idx - 1, str);
	str[idx] = '0' + (char)(n % 10);
}

char	*ft_itoa(int n)
{
	int			len;
	char		*str;
	char		is_neg;
	long long	_n;

	_n = n;
	is_neg = 0;
	if (_n < 0)
	{
		_n = -_n;
		is_neg = 1;
	}
	len = ft_num_length(_n) + is_neg;
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	if (is_neg)
		str[0] = '-';
	ft_put_digit(_n, len - 1, str);
	return (str);
}

static int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int	str_size;
	char	*new_str;

	if (s1 && s2)
	{
		str_size = ft_strlen(s1) + ft_strlen(s2) + 1;
		new_str = (char *)malloc(str_size);
		if (new_str)
		{
			new_str[str_size - 1] = 0;
			while (*s1)
				*new_str++ = *s1++;
			while (*s2)
				*new_str++ = *s2++;
			return (new_str - str_size + 1);
		}
	}
	return (NULL);
}
