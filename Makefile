NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

OBJ_DIR = obj

SRC = 	ConfigClasses/Config.cpp \
		ConfigClasses/Location.cpp \
		ConfigClasses/ServerConfig.cpp \
		Source/Source.cpp \
		Source/StaticFileSource.cpp \
		Connection.cpp \
		ListeningSocket.cpp \
		main.cpp \
		RequestParser.cpp \
		ServerConfig.cpp \
		Server.cpp \
		Socket.cpp \
		SystemCallsUtilities.cpp \
		WebServUtils.cpp \
		Tests/TestSource.cpp

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

HDR = 	ConfigClasses/Config.hpp \
		ConfigClasses/Location.hpp \
		ConfigClasses/ServerConfig.hpp \
		Source/Source.hpp \
		Source/StaticFileSource.hpp \
		Connection.hpp \
		ListeningSocket.hpp \
		RequestParser.hpp \
		HttpRequest.hpp \
		HttpResponse.hpp \
		ServerConfig.hpp \
		Server.hpp \
		Socket.hpp \
		SystemCallsUtilities.hpp \
		WebServUtlis.hpp

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.cpp $(HDR) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

