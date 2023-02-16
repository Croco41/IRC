NAME	=	ircserv

INC		=	Includes/
OBJDIR	=	Objs/
SRCDIR	=	Srcs/

CXC		=	c++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 
CINC	=	-I $(INC)
CDEP	=	-MMD -MP
# -Wshadow : Warn whenever a local variable or type declaration shadows another 
#             variable, parameter, type or class member
CFA		=	-fsanitize=address -g3
RM		=	rm -rf

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
			Srcs/Command/CapCommand.cpp

OBJ		=	$(SRC:$(SRCDIR)%.cpp=$(OBJDIR)%.o)
DEP		=	$(OBJ:.o=.d)

all			:	$(NAME)

$(NAME)		:	$(OBJ)
				$(CXC) $(CFLAGS) $^ -o $(NAME)

<<<<<<< HEAD
obj			:			
			@if [ ! -d "./Objs/Command" ]; then\
				echo "mkdir -p Objs/Command";\
				mkdir -p Objs/Command;\
			fi
				@echo ""

Objs/%.o	:	Srcs/%.cpp | obj
				$(CXC) -o $@ -c $< $(CFLAGS) $(CDEP)
=======
$(OBJDIR)%.o:	$(SRCDIR)%.cpp | $(OBJDIR)
				$(CXC) $(CDEP) $(CFLAGS) -c $< -o $@ $(CINC)

$(OBJDIR)	:	; mkdir -p $(OBJDIR)Command
>>>>>>> 04db12d456b8055381ab46c041fd4fa1bb58691b

clean		: 
				$(RM) $(OBJ)
				$(RM) $(DEP)
				$(RM) $(OBJDIR)

fclean		:	clean
				$(RM) $(NAME)

fsa			:	fclean $(OBJ)
				$(CXC) $(CFLAGS) $(CFA) -o $(NAME) $(SRC)

re			:	fclean all

<<<<<<< HEAD
.PHONY		:	all clean fclean fsa re
=======
$(DEP):
-include $(DEP)

.PHONY		:	all clean fclean fsa re
>>>>>>> 04db12d456b8055381ab46c041fd4fa1bb58691b
