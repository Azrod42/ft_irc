#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include<iostream>
#include <unistd.h>
#include <string.h>
#include<string>
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

#define	FINDUSER std::vector<t_user>::iterator it = _user.begin();\
	while (it != _user.end()){ \
		if (it->id == id){ \
			break; \
		} \
		it++; \
	}

typedef struct s_data {
	unsigned short	port;
	std::string		pass;
	int				rc;
	int				fd_socket;
} t_data;

typedef struct s_uer {
	std::string		cmd;
	unsigned int	id;
	std::string		nick;
	std::string		name;//user
	std::string		realname;
	bool			pass_ok;
	bool			nick_ok;
	bool			user_ok;
	int				is_log;
	std::string		mode;
	std::string		unused;
} t_user;

char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(const char *s, unsigned int start, size_t len);
int				ispass(int c);
int				check_nickname(std::string input);
int				check_empty(std::string input);
std::string		rplwelcome(std::string nick, std::string user);
std::string		error_alreadyregistred();
std::string		error_pass(std::string pass);
std::string		error_needmoreparams(std::string word);
std::string		error_erroneusnickname(std::string nick);
std::string		error_unknowcommand(std::string cmd);
std::string		error_nonicknamegiven(void);
std::string		error_nicknameinuse(std::string nick);

#endif