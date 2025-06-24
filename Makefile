SRC = philo_tests.c ft_atoi.c
OBJ = $(SRC:.o=.c)
CC = cc
FLAGS = -Wall -Werror -Wextra -fsanitize=thread
NAME = philo



all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)
%.o : %.c  threads.h
	$(CC) $(FLAGS)  $< -o $@