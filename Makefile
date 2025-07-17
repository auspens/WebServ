NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98
DEBUG_FLAGS = -g -O0
CFLAGS += $(DEBUG_FLAGS)

SRCDIR = src
OBJDIR = obj

SRC =	$(SRCDIR)/Config/Config.cpp \
		$(SRCDIR)/Config/Location.cpp \
		$(SRCDIR)/Config/ServerConfig.cpp \
		$(SRCDIR)/Config/ParseUtils.cpp \
		$(SRCDIR)/Config/ConfigSettings.cpp \
		$(SRCDIR)/Config/ConfigParseException.cpp \
		$(SRCDIR)/Logger/Logger.cpp \
		$(SRCDIR)/Logger/LogStream.cpp \
		$(SRCDIR)/Source/Source.cpp \
		$(SRCDIR)/Source/StaticFileSource.cpp \
		$(SRCDIR)/Source/ErrorPageSource.cpp \
		$(SRCDIR)/Source/RedirectSource.cpp \
		$(SRCDIR)/Source/CGISource.cpp \
		$(SRCDIR)/Source/IsChildProcessException.cpp \
		$(SRCDIR)/Source/PageGenerator.cpp \
		$(SRCDIR)/Source/StatusCodesStorage.cpp \
		$(SRCDIR)/Source/MimeTypeInitialiser.cpp \
		$(SRCDIR)/Source/SourceAndRequestException.cpp \
		$(SRCDIR)/Source/SourceFactory.cpp \
		$(SRCDIR)/Source/UploadSource.cpp \
		$(SRCDIR)/Source/DeleteSource.cpp \
		$(SRCDIR)/Source/ShutDownSource.cpp \
		$(SRCDIR)/Server/Connection.cpp \
		$(SRCDIR)/Server/ListeningSocket.cpp \
		$(SRCDIR)/main.cpp \
		$(SRCDIR)/Request/RequestParser.cpp \
		$(SRCDIR)/Server/Server.cpp \
		$(SRCDIR)/Server/Socket.cpp \
		$(SRCDIR)/Utils/SystemCallsUtilities.cpp \
		$(SRCDIR)/Utils/WebServUtils.cpp

HDR =	$(SRCDIR)/Config/Config.hpp \
		$(SRCDIR)/Config/Location.hpp \
		$(SRCDIR)/Config/ServerConfig.hpp \
		$(SRCDIR)/Config/ParseUtils.hpp \
		$(SRCDIR)/Config/Constants.hpp \
		$(SRCDIR)/Config/ConfigSettings.hpp \
		$(SRCDIR)/Config/ConfigParseException.hpp \
		$(SRCDIR)/Logger/Logger.hpp \
		$(SRCDIR)/Logger/LogStream.hpp \
		$(SRCDIR)/Source/Source.hpp \
		$(SRCDIR)/Source/StaticFileSource.hpp \
		$(SRCDIR)/Source/RedirectSource.hpp \
		$(SRCDIR)/Source/CGISource.hpp \
		$(SRCDIR)/Source/IsChildProcessException.hpp \
		$(SRCDIR)/Source/PageGenerator.hpp \
		$(SRCDIR)/Source/StatusCodesStorage.hpp \
		$(SRCDIR)/Source/SourceAndRequestException.hpp \
		$(SRCDIR)/Source/SourceFactory.hpp \
		$(SRCDIR)/Source/UploadSource.hpp \
		$(SRCDIR)/Source/DeleteSource.hpp \
		$(SRCDIR)/Source/ShutDownSource.hpp \
		$(SRCDIR)/Server/Connection.hpp \
		$(SRCDIR)/Server/ListeningSocket.hpp \
		$(SRCDIR)/Request/RequestParser.hpp \
		$(SRCDIR)/Request/HttpRequest.hpp \
		$(SRCDIR)/Server/Server.hpp \
		$(SRCDIR)/Server/Socket.hpp \
		$(SRCDIR)/Utils/SystemCallsUtilities.hpp \
		$(SRCDIR)/Utils/WebServUtils.hpp

OBJ = $(patsubst %.cpp,$(OBJDIR)/%.obj,$(notdir $(SRC)))

INCLUDE = -I. -I$(SRCDIR)/Config  -I$(SRCDIR)/Logger -I$(SRCDIR)/Request -I$(SRCDIR)/Server -I$(SRCDIR)/Source -ITests -I$(SRCDIR)/Utils

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

$(OBJDIR)/%.obj: $(SRCDIR)/*/%.cpp $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/%.obj: $(SRCDIR)/%.cpp $(HDR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
