COMPILER = c++

FLAGS = -Wall -Wextra -Werror 

NAME = PmergeMe

SCRS = PmergeMe.cpp main.cpp

OBJS = $(SCRS:.cpp=.o)

%.o : %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME) : $(OBJS)
	$(COMPILER) $(FLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean	
	rm -f $(NAME)

re: fclean all 

.PHONY: all clean fclean re 

.SECONDARY: $(OBJS)