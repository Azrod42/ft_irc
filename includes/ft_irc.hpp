#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include<iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>

#define ERR_UNKNOWCOMMAND " :Unknow command\n"
#define ERR_ALREADYREGISTRED " :Unauthorized command (already registered)\n"
#define ERR_NEEDMOREPARAMS " :Not enough parameters\n" // need <command> avant l'erreur

typedef struct s_data {
	unsigned short	port;
	std::string		pass;
	int				rc;
	int				fd_socket;
} t_data;

typedef struct s_uer {
	unsigned int	id;
	std::string		nick;
	std::string		name;//user
	std::string		real;
	int				pass_ok;
	int				is_log;
} t_user;

char	**ft_split(char const *s, char c);
int		ispass(int c);

#endif