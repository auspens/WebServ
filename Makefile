NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98
DEBUG_FLAGS = -g -O0
CFLAGS += $(DEBUG_FLAGS)

OBJ_DIR = obj

SRC = 	Config/Config.cpp \
		Config/Location.cpp \
		Config/ServerConfig.cpp \
		Config/ParseUtils.cpp \
		Config/ConfigSettings.cpp \
		Config/ConfigParseException.cpp \
		Response.cpp \
		Source/Source.cpp \
		Source/StaticFileSource.cpp \
		Source/ErrorPageSource.cpp \
		Source/RedirectSource.cpp \
		Source/CGISource.cpp \
		Source/ChildProcessNeededException.cpp \
		Source/StatusCodesInitialiser.cpp \
		Source/MimeTypeInitialiser.cpp \
		Connection.cpp \
		ListeningSocket.cpp \
		main.cpp \
		RequestParser.cpp \
		Server.cpp \
		Socket.cpp \
		SystemCallsUtilities.cpp \
		WebServUtils.cpp \
		Tests/TestConfig.cpp \
		Tests/TestUtils.cpp

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

HDR = 	Config/Config.hpp \
		Config/Location.hpp \
		Config/ServerConfig.hpp \
		Config/ParseUtils.hpp \
		Config/Constants.hpp \
		Config/ConfigSettings.hpp \
		Config/ConfigParseException.hpp \
		Response.hpp \
		Source/Source.hpp \
		Source/StaticFileSource.hpp \
		Source/RedirectSource.hpp \
		Source/CGISource.hpp \
		Source/ChildProcessNeededException.hpp \
		Connection.hpp \
		ListeningSocket.hpp \
		RequestParser.hpp \
		HttpRequest.hpp \
		HttpResponse.hpp \
		Server.hpp \
		Socket.hpp \
		SystemCallsUtilities.hpp \
		WebServUtlis.hpp \
		Tests/TestConfig.hpp \
		Tests/TestUtils.hpp

INCLUDE = -I. -IConfig -ISource -ITests

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.cpp $(HDR) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

