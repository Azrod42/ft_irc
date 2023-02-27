NAME =container

SRCS = srcs/main.cpp \

INCLUDE = includes/* 

OBJS		=$(SRCS:.cpp=.o)
CC			=c++
CPPFLAGS	=-std=c++98
FLAGS		=-Wall -Werror -Wextra ${CPPFLAGS}
INCS		=.

.cpp.o :
	@echo "${_BOLD}${_BLUE}Making ${NAME} > ${_END}${_YELLOW}${<:.cpp=.o}${_END}"
	@${CC} ${FLAGS} -c $< -o ${<:.cpp=.o} -I${INCS}

all: $(NAME)

${NAME}: flag_use ${OBJS}
	@echo "${_BOLD}✅ ${_IGREEN} ${NAME} > *.o Done${_END}"
	${_STATUS2}
	@${CC} ${FLAGS} ${OBJS} -o ${NAME}
	${_DONE}
	${_PRINTART1}
	${_USAGE}

clean:
	${_STATUS3}
	@rm -rf *.o
	${_DONE}

fclean: clean
	${_STATUS5}
	@rm -rf $(NAME)
	${_DONE}

norm:
	${_PRINTART2}
	@norminette ${SRCS} ${INCLUDE}

git_add:
	${_STATUS7}	
	git add ${SRCS} ${INCLUDE} Makefile
	${_DONE}
	@git status | grep modif

git_push:
	${_STATUS8}	
	@git commit -m "Autopush Makefile"
	${_DONE}
	${_STATUS9}	
	@git push origin master
	${_DONE}

git_master: git_add git_push

re: fclean all

ree: fclean all
	@./${NAME}

reee:
	@./${NAME}

flag_use:
	@echo "${_BOLD}${_BLUE}============================= FLAGS USE =============================${_END}"
	@echo "${_BOLD}${_RED}\t\t   ${FLAGS}${_END}"
	@echo "${_BOLD}${_BLUE}=====================================================================${_END}"

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# This is a minimal set of ANSI/VT100 color codes
_END=$'\x1b[0m
_BOLD=$'\x1b[1m
_UNDER=$'\x1b[4m
_REV=$'\x1b[7m

# Colors
_GREY=$'\x1b[30
_RED=$'\x1b[31m
_GREEN=$'\x1b[32m
_YELLOW=$'\x1b[33m
_BLUE=$'\x1b[34m
_PURPLE=$'\x1b[35m
_CYAN=$'\x1b[36m
_WHITE=$'\x1b[37m

# Inverted, i.e. colored backgrounds
_IGREY=$'\x1b[40m
_IRED=$'\x1b[41m
_IGREEN=$'\x1b[42m
_IYELLOW=$'\x1b[43m
_IBLUE=$'\x1b[44m
_IPURPLE=$'\x1b[45m
_ICYAN=$'\x1b[46m
_IWHITE=$'\x1b[47m
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

_PRINTART1=@echo "${_BOLD}${_YELLOW}${_L0}\n${_L1}\n${_L2}\n${_L3}\n${_L4}\n${_L5}\n${_L0}\n${_END}"
_L0=$'|----------------------------------------|
_L1=$'| ___ ____ ____ ____ ____ ___  ____ _    |
_L2=$'|  |  [__  |  | |__/ |__| |__] |___ |    |
_L3=$'|  |  ___] |__| |  \ |  | |__] |___ |___ |
_L4=$'|                                        |
_L5=$'|      |f|t|_|c|o|n|t|a|n|e|r|s|         |

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

_PRINTART2=@echo "${_BOLD}${_RED}${_L08}\n${_L09}\n${_L10}\n${_L11}\n${_L12}\n${_L13}\n${_L14}\n${_L08}\n${_END}"
_L08=$'|-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-|'
_L09=$'| ███╗   ██╗ ██████╗ ██████╗ ███╗   ███╗██╗███╗   ██╗███████╗████████╗████████╗███████╗ |
_L10=$'| ████╗  ██║██╔═══██╗██╔══██╗████╗ ████║██║████╗  ██║██╔════╝╚══██╔══╝╚══██╔══╝██╔════╝ |
_L11=$'| ██╔██╗ ██║██║   ██║██████╔╝██╔████╔██║██║██╔██╗ ██║█████╗     ██║      ██║   █████╗   |
_L12=$'| ██║╚██╗██║██║   ██║██╔══██╗██║╚██╔╝██║██║██║╚██╗██║██╔══╝     ██║      ██║   ██╔══╝   |
_L13=$'| ██║ ╚████║╚██████╔╝██║  ██║██║ ╚═╝ ██║██║██║ ╚████║███████╗   ██║      ██║   ███████╗ |
_L14=$'| ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝   ╚═╝      ╚═╝   ╚══════╝ |

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

_USAGE=@echo "${_BOLD}${_RED}\nUsage : ./${NAME}${_END}"

_DONE=@echo "${_BOLD}✅ ${_IGREEN}Done${_END}"
_STATUS2=@echo "${_BOLD}${_BLUE}Making > ${_END}${_YELLOW} ${NAME}${_END}"  
_STATUS3=@echo "${_BOLD}${_BLUE}Removing > ${_END}${_YELLOW} ${OBJS} ${_END}"  
_STATUS5=@echo "${_BOLD}${_BLUE}Removing > ${_END}${_RED} ${_GREEN}$(NAME)${_END}"  
_STATUS6=@echo "${_BOLD}${_BLUE}Removing > ${_END}${_YELLOW} $(OBJS) $(NAME) ${_END}"  
_STATUS7=@echo "${_BOLD}${_BLUE}GIT ADD : ${SRCS} ${INCLUDE} Makefile ${_END}"  
_STATUS8=@echo "${_BOLD}${_BLUE}GIT COMMIT ...${_END}"  
_STATUS9=@echo "${_BOLD}${_BLUE}GIT PUSH ...${_END}"  

