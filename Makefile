NAME		=	webserv

INCLUDE 	=	-I./include -I./include/HTTP -I./include/HTTPConfig

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 $(INCLUDE)

SRC			=	main.cpp \
				ServerBlock.cpp \
				ServerManager.cpp \
				LocationBlock.cpp \
				Monitor.cpp \
				System.cpp \
				KQueue.cpp \
				Server.cpp \
				Client.cpp \
				utils.cpp \
				HTTP/Header.cpp \
				HTTP/RequestHeader.cpp \
				HTTP/ResponseHeader.cpp \
				HTTPConfig/HTTPEntityHeader.cpp \
				HTTPConfig/HTTPGeneralHeader.cpp \
				HTTPConfig/HTTPConfig.cpp \


SRC_DIR 	=	./src
SRCS		=	$(addprefix $(SRC_DIR),$(SRC))
OBJ_DIR		=	./obj
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