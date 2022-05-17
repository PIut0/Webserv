NAME			=	kqueue_practice

INCLUDE 		=	-I./include

CXX				=	c++
CXXFLAGS		=	-Wall -Wextra -Werror -std=c++98 $(INCLUDE)


SRCS			=	main.cpp \
					kqueue.cpp \
					server.cpp \
					client.cpp \
					util.cpp

SRC_DIR 		=	./src
SRCS			=	$(addprefix $(SRC_DIR),$(SRC))

OBJ_DIR			=	./obj
OBJS			=	$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(SRC)))


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

re		:
	make fclean
	make all

.PHONY	:	all clean fclean re bonus norm
