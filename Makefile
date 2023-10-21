NAME = webserver

SRC =	src/main.cpp \
		src/Token/Tokenizer.cpp \
		src/Print/printTokens.cpp \







HEADER =	include/includes.hpp \
			include/Tokenizer.hpp \








OBJ = $(SRC:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

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
