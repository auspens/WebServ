NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

OBJ_DIR = obj

SRC = 	Config.cpp \
		Connection.cpp \
		ListeningSocket.cpp \
		main.cpp \
		Request.cpp \
		ServerConfig.cpp \
		Server.cpp \
		Socket.cpp \
		SystemCallsUtilities.cpp

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

HDR = 	Config.hpp \
		Connection.hpp \
		ListeningSocket.hpp \
		Request.hpp \
		ServerConfig.hpp \
		Server.hpp \
		Socket.hpp \
		SystemCallsUtilities.hpp

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

