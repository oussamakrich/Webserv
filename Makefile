NAME = webserver

SRC = main.cpp

HEADER = include/includes.hpp \
				include/Server.hpp \
				include/ParseConfig.hpp \
				
OBJDIR = obj/

OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

FLAGS = -Wall -Wextra -Werror -std=c++98

all: $(OBJDIR) $(NAME)

$(OBJDIR):
                mkdir -p $(OBJDIR)

$(NAME): $(OBJ) $(HEADER)
                c++ $(FLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o: %.cpp $(HEADER)
                c++ $(FLAGS) -c $< -o $@

clean:
                rm -rf $(OBJDIR)

fclean: clean
                rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
