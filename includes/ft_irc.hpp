#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include<iostream>
#include <unistd.h>
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

typedef struct s_data {
	unsigned short	port;
	int				rc;
	int				fd_socket;
} t_data;

typedef struct s_user {
	unsigned int	id;
	std::string		name;
} t_user;
#endif