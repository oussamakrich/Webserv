NAME = webserver

SRC =	./HttpElement/src/Client.cpp\
		./HttpElement/src/Global.cpp\
		./HttpElement/src/Location.cpp\
		./HttpElement/src/Server.cpp\
		./HttpElement/src/ServerFd.cpp\
		./main.cpp\
		./Parsing/src/GenerateGlobalClass.cpp\
		./Parsing/src/GenerateLocation.cpp\
		./Parsing/src/GenerateServer.cpp\
		./Parsing/src/Tokenizer.cpp\
		./Utils/src/Logger.cpp\
		./Utils/src/printTokens.cpp\
		./Utils/src/trim.cpp\
		./Utils/src/UrlEncoder.cpp\
		./Utils/src/DirListing.cpp\
		./ErrorResponse/src/ErrorResponse.cpp\
		./ErrorResponse/src/GenerateError.cpp\
		./Parsing/src/ParsRequest.cpp\
		./Request/Buffer/RequestBuffer.cpp\
		./Request/Buffer/Byte.cpp\
		./HttpElement/src/Request.cpp\
		./Response/src/Response.cpp\
		./Response/src/GenerateResponse.cpp\
		./Response/src/AllMethod.cpp\
		./HttpElement/src/Cgi.cpp\
		./Utils/src/signal.cpp\
		./Uploader/src/Upload.cpp \




HEADER =	./HttpElement/include/Client.hpp \
			./HttpElement/include/Global.hpp \
			./HttpElement/include/Location.hpp \
			./HttpElement/include/Server.hpp \
			./include/includes.hpp \
			./Parsing/include/GenerateGlobalClass.hpp \
			./Parsing/include/GenerateLocation.hpp \
			./Parsing/include/GenerateServer.hpp \
			./Parsing/include/Tokenizer.hpp \
			./Utils/include/Logger.hpp\
			./Utils/include/DirListing.hpp\
			./ErrorResponse/include/ErrorResponse.hpp\
			./ErrorResponse/include/GenerateError.hpp\
			./Parsing/include/ParsRequest.hpp\
			./Request/Buffer/RequestBuffer.hpp\
			./HttpElement/include/Request.hpp\
			./Response/include/Response.hpp\
			./Response/include/GenerateResponse.hpp\
			./Response/include/AllMethod.hpp\
			./HttpElement/include/Cgi.hpp\
			./Request/Buffer/Byte.hpp\
			./Uploader/include/Upload.hpp \



OBJ = $(SRC:.cpp=.o)

FLAGS = -g #-fsanitize=address -g #-Wall -Wextra -Werror -std=c++98

all:  $(NAME)

$(NAME): $(OBJ) $(HEADER)
			c++ $(FLAGS) $(OBJ) -o $(NAME)
			@mkdir -p tmp
%.o: %.cpp $(HEADER)
			c++ $(FLAGS) -c $< -o $@

clean c:
			rm -rf $(OBJ)

fclean f: clean
			rm -rf $(NAME) ./tmp

re: fclean all

.PHONY: all clean fclean re
