# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   Makefile                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2022/07/15 11:22:57 by tfelwood          #+#    #+#             #
#   Updated: 2022/07/15 11:22:57 by tfelwood         ###   ########.fr       #
#                                                                            #
# ************************************************************************** #



SRCS		=	main_bonus.c\
				utils_bonus.c\
				init_bonus.c\
				utils2_bonus.c\
				philo_bonus.c
OBJS		=	$(patsubst %.c,%.o,$(SRCS))
NAME		=	philo_bonus
CC			= 	gcc
RM			=	rm -f
CFLAGS		=	-Wall -Werror -Wextra
D_FILES		=	$(patsubst %.c,%.d,$(SRCS))

.PHONY:	all clean fclean re bonus

all:	$(NAME)

%.o:	%.c
		$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} -MMD

$(NAME):	$(OBJS)
			$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

-include	$(D_FILES)

clean:
		$(RM) $(OBJS)
		$(RM) $(D_FILES)

fclean:	clean
		${RM} $(NAME)

re:	fclean all