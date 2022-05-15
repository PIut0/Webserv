NAME			=	kqueue_practice

SRCS			=	main.cpp \
						kqueue.cpp \
						server.cpp \
						client.cpp \
						util.cpp

OBJS			= $(SRCS:.cpp=.o)

CPP				= c++
CPPFLAGS	= -Wall -Wextra -Werror -I. --std=c++98

all:			$(NAME)

$(NAME):		$(OBJS)
				${CPP} ${CPPFLAGS} -o ${NAME} ${OBJS}

clean:
				rm -rf $(OBJS)

fclean:			clean
				rm -rf $(NAME)

re:				fclean $(NAME)

.PHONY			: all clean fclean re
