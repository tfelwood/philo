# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   Makefile                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2022/07/09 21:54:16 by tfelwood          #+#    #+#             #
#   Updated: 2022/07/09 21:54:16 by tfelwood         ###   ########.fr       #
#                                                                            #
# ************************************************************************** #



SRCS		=	main.c\
				utils.c\
				init.c\
				philo.c
OBJS		=	$(patsubst %.c,%.o,$(SRCS))
NAME		=	philo
CC			= 	gcc
RM			=	rm -f
CFLAGS		=	-Wall -Werror -Wextra - MMD
D_FILES		=	$(patsubst %.c,%.d,$(SRCS))

.PHONY:	all clean fclean re bonus

all:	$(NAME)

#%.o:	%.c
#		$(CC) $(CFLAGS) -c $< -o ${<:.c=.o} -MMD

$(NAME):	$(OBJS)
			$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

-include	$(D_FILES)

clean:
		$(RM) $(OBJS)
		$(RM) $(D_FILES)

fclean:	clean
		${RM} $(NAME)

re:	fclean all