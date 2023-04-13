#pragma once

#include "ft_irc.hpp"
#include "User.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <vector>

class Server
{
public:
	Server(void);
	Server(char *port, char *pass);
	Server(const Server & src);
	Server&	operator=(const Server & rhs);
	~Server();

	// GETTERS
	int	getServerFD() const;

	// ACTIONS
	int		CreateServer();
	int		StartServer(User &user);

private:
	int							_port;
	std::string					_password;
	int							_socketFd;
	struct sockaddr_in			_addr;
	std::vector<struct pollfd>	_fds;
	int							_timeout;
	bool						_ON;
};
