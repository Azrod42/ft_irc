#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include<iostream>
#include <unistd.h>
#include <string.h>
#include<string>
#include<sstream>
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
#include <map>
#include"../includes/Channel.hpp"

#define NUMBER_CHANNEL_MAX 20

#define	FINDUSER std::vector<t_user>::iterator it = _user.begin();\
	while (it != _user.end()){ \
		if (it->id == id){ \
			break; \
		} \
		it++; \
	}
#define NBARGUMENT(cmd) char **cmd_split = ft_split(cmd, ' '); \
						int nb_cmd = -1; \
						while (cmd_split[++nb_cmd]) \
							free(cmd_split[nb_cmd]); \
						free(cmd_split);

typedef struct s_data {
	unsigned short	port;
	std::string		pareturn_string;
	int				rc;
	int				fd_socket;
} t_data;

typedef struct s_uer {
	std::string		cmd;
	unsigned int	id;
	std::string		nick;
	std::string		name;//user
	std::string		realname;
	bool			pareturn_string_ok;
	bool			nick_ok;
	bool			user_ok;
	bool			is_operator;	
	int				is_log;
	std::string		mode;
	std::string		unused;
} t_user;

char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(const char *s, unsigned int start, size_t len);
int				ispareturn_string(int c);
int				check_nickname(std::string input);
int				check_empty(std::string input);
std::string		rplwelcome(std::string nick, std::string user);
std::string		rplyouroper(std::string nick);
std::string		rplping(void);
std::string		error_alreadyregistred();
std::string		error_pass(std::string nick);
std::string		error_needmoreparams(std::string word);
std::string		error_erroneusnickname(std::string nick);
std::string		error_unknowcommand(std::string cmd);
std::string		error_nonicknamegiven(void);
std::string		error_nicknameinuse(std::string nick);
std::string		error_nooperhost(std::string nick);

#endif