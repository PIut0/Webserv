NAME			=	webserv

INCLUDE 	=	-I./include -I./include/http -I./include/config -I./include/error -I./include/common -I./include/debug -I./include/core -I./include/core/method -I./include/util

CXX				=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 $(INCLUDE)

MAIN			= main.cpp

SRC				=	core/KQueue.cpp \
						core/Server.cpp \
						core/Client.cpp \
						core/Method/Method.cpp \
						core/Method/GetMethod.cpp \
						core/Method/PutMethod.cpp \
						core/Method/PostMethod.cpp \
						util/utils.cpp \
						util/mimes.cpp \
						util/process.cpp \
						debug/System.cpp \
						debug/Monitor.cpp \
						http/Header.cpp \
						http/RequestHeader.cpp \
						http/ResponseHeader.cpp \
						config/ServerBlock.cpp \
						config/ServerManager.cpp \
						config/LocationBlock.cpp \


SRC_DIR 	=	./src
OBJ_DIR		=	./obj
TEST_DIR	= ./test

SRCS		= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS		=	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(SRC)))

ifdef TEST
SRCS		+=  $(TEST)
OBJS		+=	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(TEST)))
else
SRCS		+=  $(MAIN)
OBJS		+=	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(MAIN)))
endif

SHELL		=	/bin/bash

all		:	$(NAME)

ifdef TEST
$(OBJ_DIR)/%.o	: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
endif

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
