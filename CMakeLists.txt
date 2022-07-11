cmake_minimum_required(VERSION 3.20)
project(philo C)

set(CMAKE_C_STANDARD 99)
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -fsanitize=thread")

add_executable(philo philo/main.c philo/philo.c philo/init.c philo/utils.c)

add_executable(philo_bonus philo_bonus/main_bonus.c philo_bonus/init_bonus.c philo_bonus/utils_bonus.c philo_bonus/philo_bonus.c philo_bonus/utils2_bonus.c)