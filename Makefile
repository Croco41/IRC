NAME	=	ircserv

INC		=	Includes

CXC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -I $(INC)
CDEP	=	-MMD
# -Wshadow : Warn whenever a local variable or type declaration shadows another 
#             variable, parameter, type or class member
CFA		=	-fsanitize=address -g3
RM		=	rm -rf

# vpath %.cpp Srcs Srcs/Command

SRC		=	Srcs/main.cpp \
			Srcs/server.cpp \
			Srcs/client.cpp \
			Srcs/channel.cpp \
			Srcs/commandhandler.cpp \
			Srcs/Command/JoinCommand.cpp \
			Srcs/Command/PingCommand.cpp \
			Srcs/Command/PongCommand.cpp \
			Srcs/Command/PrivMsgCommand.cpp \
			Srcs/Command/NoticeCommand.cpp \
			Srcs/Command/PartCommand.cpp \
			Srcs/Command/UserCommand.cpp \
			Srcs/Command/PassCommand.cpp \
			Srcs/Command/NickCommand.cpp \
			Srcs/Command/ModeCommand.cpp \
			Srcs/Command/QuitCommand.cpp \
			Srcs/Command/WhoCommand.cpp \
			Srcs/Command/WhoisCommand.cpp \
			Srcs/Command/OperCommand.cpp \
			Srcs/Command/TopicCommand.cpp \
			Srcs/Command/KickCommand.cpp \
			Srcs/Command/KillCommand.cpp \
			Srcs/Command/NamesCommand.cpp \
			Srcs/Command/ListCommand.cpp \
			Srcs/Command/CapCommand.cpp \

OBJ		=	$(SRC:Srcs/%.cpp=Objs/%.o)
DEP		=	$(SRC:Srcs/%.cpp=Objs/%.d)


all			:	$(NAME)

$(DEP):
-include $(DEP)

$(NAME)		:	$(OBJ)
				$(CXC) $(CFLAGS) -o $(NAME) $(SRC)

obj			:	; mkdir -p Objs/Command
# @if [ ! -d "./Objs" ]; then\
# 	echo "mkdir -p Objs";\
# 	mkdir -p Objs;\
# fi
# 	@echo ""

Objs/%.o	:	Srcs/%.cpp Objs/%.d | obj
				$(CXC) -o $@ -c $< $(CFLAGS) $(CDEP)

clean		: 
				$(RM) $(OBJ)
				$(RM) $(DEP)
				$(RM) Objs

fclean		:	clean
				$(RM) $(NAME)

fsa			:	fclean $(OBJ)
				$(CXC) $(CFLAGS) $(CFA) -o $(NAME) $(SRC)

re			:	fclean all

.PHONY		:	all obj clean fclean fsa re