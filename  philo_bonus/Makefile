SRC = bonus.c ft_atoi.c ft_strjoin.c ft_itoa.c
OBJ = $(SRC:.c=.o)
CC = cc
FLAGS = #-Wall -Werror -Wextra
NAME = philo_bonus

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