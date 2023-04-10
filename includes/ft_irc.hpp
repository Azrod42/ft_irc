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

#define NUMBER_CHANNEL_MAX 200
#define IP INADDR_ANY
#define BACKLOG 3
#define TIMEOUT_M 2
#define TIMEOUT_S 30
#define MAXPORT 65535
#define BUFFER_LEN 2000
#define NB_CLIENT 256

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
	std::string		ip;
} t_user;

char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(const char *s, unsigned int start, size_t len);
int				ispareturn_string(int c);
int				check_nickname(std::string input);
int				check_empty(std::string input);
std::string		ip_itostr(in_addr_t brut);
std::string		rpl(std::string nick, std::string channel, std::string name, std::string message);
std::string		rplkill(std::string nick, std::string reason);
std::string		rplwelcome(std::string nick, std::string user);
std::string		rplkicked(std::string nick, std::string channel, std::string reason);
std::string		rplyouroper(std::string nick);
std::string		rplping(void);
std::string		rpljoin(std::string nick, std::string name, std::string channel);
std::string		rplnotopic(std::string channel_name);
std::string		rpltopic(std::string channel_name, std::string topic);
std::string		error_alreadyregistred();
std::string		error_pass(std::string nick);
std::string		error_needmoreparams(std::string word);
std::string		error_erroneusnickname(std::string nick);
std::string		error_unknowcommand(std::string cmd);
std::string		error_nonicknamegiven(void);
std::string		error_nicknameinuse(std::string nick);
std::string		error_nooperhost(std::string nick);
std::string		error_toomanychannels(std::string channel_name);
std::string		error_badchannelkey(std::string channel_name);
std::string		error_inviteonlychan(std::string channel_name);
std::string		error_bannedformchan(std::string channel_name);
std::string		error_notonchannel(std::string channel_name);
std::string		error_nosuchchannel(std::string channel_name);
std::string		error_cannotsendtochan(std::string channel_name);
std::string		error_norecipient(std::string commande);
std::string		error_notexttosend(void);
std::string		error_nosuchnick(std::string nick);
std::string		error_usernotinchannel(std::string nick, std::string channel);
std::string		error_chanoprivsneeded(std::string chann);
std::string		error_noprivileges(void);

#endif