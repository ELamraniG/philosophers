SRC = philo_test.c ft_atoi.c
OBJ = $(SRC:.c=.o)
CC = cc
FLAGS = -Wall -Werror -Wextra #-fsanitize=thread
NAME = philo



all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)
%.o : %.c  threads.h
	$(CC) -c $(FLAGS)  $< -o $@

clean : 
	rm -f $(OBJ) 
fclean : clean
	rm -f $(NAME)
re : fclean all