NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98
DEBUG_FLAGS = -g -O0
CFLAGS += $(DEBUG_FLAGS)

OBJDIR = Obj

SRC =	Config/Config.cpp \
		Config/Location.cpp \
		Config/ServerConfig.cpp \
		Config/ParseUtils.cpp \
		Config/ConfigSettings.cpp \
		Config/ConfigParseException.cpp \
		Logger/Logger.cpp \
		Logger/LogStream.cpp \
		Response.cpp \
		Source/Source.cpp \
		Source/StaticFileSource.cpp \
		Source/ErrorPageSource.cpp \
		Source/RedirectSource.cpp \
		Source/CGISource.cpp \
		Source/ChildProcessNeededException.cpp \
		Source/StatusCodesInitialiser.cpp \
		Source/MimeTypeInitialiser.cpp \
		Source/SourceAndRequestException.cpp \
		Source/SourceFactory.cpp \
		Source/UploadSource.cpp \
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

HDR =	Config/Config.hpp \
		Config/Location.hpp \
		Config/ServerConfig.hpp \
		Config/ParseUtils.hpp \
		Config/Constants.hpp \
		Config/ConfigSettings.hpp \
		Config/ConfigParseException.hpp \
		Logger/Logger.hpp \
		Logger/LogStream.hpp \
		Response.hpp \
		Source/Source.hpp \
		Source/StaticFileSource.hpp \
		Source/RedirectSource.hpp \
		Source/CGISource.hpp \
		Source/ChildProcessNeededException.hpp \
		Source/SourceAndRequestException.hpp \
		Source/SourceFactory.hpp \
		Source/UploadSource.hpp \
		Connection.hpp \
		ListeningSocket.hpp \
		RequestParser.hpp \
		HttpRequest.hpp \
		Response.hpp \
		Server.hpp \
		Socket.hpp \
		SystemCallsUtilities.hpp \
		WebServUtils.hpp \
		Tests/TestConfig.hpp \
		Tests/TestUtils.hpp

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.obj)

INCLUDE = -I. -IConfig -ISource -ITests -ILogger

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

$(OBJDIR)/%.obj: %.cpp $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
