NAME			=	webserv

INCLUDE 	=	-I./include -I./include/http -I./include/config -I./include/error -I./include/common -I./include/debug

CXX				=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 $(INCLUDE)

SRC				= main.cpp \
						KQueue.cpp \
						Server.cpp \
						Client.cpp \
						utils.cpp \
						debug/System.cpp \
						debug/Monitor.cpp \
						http/Header.cpp \
						http/RequestHeader.cpp \
						http/ResponseHeader.cpp \
						http/parsing.cpp \
						config/ServerBlock.cpp \
						config/ServerManager.cpp \
						config/LocationBlock.cpp \



SRC_DIR 	=	./src
OBJ_DIR		=	./obj

SRCS		= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS		=	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(SRC)))

SHELL		=	/bin/bash

all		:	$(NAME)

$(OBJ_DIR)/%.o	: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME)	:	$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bonus	: all

clean	:
	@rm -rf $(OBJ_DIR)

fclean	:	clean
	@rm -f $(NAME)

re		:	fclean	all

.PHONY	:	all clean fclean re bonus
