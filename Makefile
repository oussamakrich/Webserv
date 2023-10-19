NAME = webserver

SRC =	src/main.cpp \
		src/Token/Tokenizer.cpp







HEADER =	include/includes.hpp \
			include/Tokenizer.hpp \








OBJ = $(SRC:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++11

all:  $(NAME)

$(NAME): $(OBJ) $(HEADER)
			c++ $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
			c++ $(FLAGS) -c $< -o $@

clean c:
			rm -rf $(OBJ)

fclean f: clean
			rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
