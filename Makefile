NAME = webserver

SRC =	./src/Global/Global.cpp \
			./src/Location/Location.cpp \
			./src/main.cpp \
			./src/Parse/GenerateGlobalClass.cpp \
			./src/Parse/GenerateLocation.cpp\
			./src/Server/Server.cpp\
			./src/Token/Tokenizer.cpp


HEADER =./include/GenerateGlobalClass.hpp \
				./include/GenerateLocation.hpp\
				./include/Global.hpp\
				./include/includes.hpp\
				./include/Location.hpp\
				./include/Server.hpp\
				./include/Tokenizer.hpp


OBJ = $(SRC:.cpp=.o)

FLAGS = -g#-Wall -Wextra -Werror -std=c++98 -g

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
