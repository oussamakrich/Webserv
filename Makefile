NAME = webserver

SRC =	src/main.cpp \
		src/config/GenerateServers.cpp \
		src/config/ParseConfig.cpp \
		src/config/TokenConfig.cpp \
		src/server/Server.cpp \
		src/utils/trim.cpp \
		src/serverlog/Logger.cpp \
		src/Location/Location.cpp \
		src/Request/Request.cpp \

HEADER =	include/includes.hpp \
			include/GenerateServers.hpp \
			include/ParseConfig.hpp \
			include/Server.hpp \
			include/TokenConfig.hpp \
			include/Logger.hpp \
			include/Location.hpp \
			include/Request.hpp \


OBJ = $(SRC:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

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
