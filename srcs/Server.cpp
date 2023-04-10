#include "../includes/Server.hpp"

//*--------------[ Constructor ]-----------------*//

Server::Server()
{

}

Server::Server(char *port, char *pass) : _port(std::stoi(port)), _password(pass)
{

}

Server::Server(const Server & src)
{
	*this=src;
}

Server&	Server::operator=(const Server & rhs)
{
	this->_port = rhs._port;
	this->_password = rhs._password;
	this->_socketFd = rhs._socketFd;
	return (*this);
}

Server::~Server()
{

}

//*--------------[ Actions ]-----------------*//

int	Server::getServerFD() const
{
	return (this->_socketFd);
}

//*--------------[ Actions ]-----------------*//

int	Server::CreateServer()
{
	int	tempFd = 0;
	int		on = 1;
	tempFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (tempFd == -1)
	{
		std::cout << "\033[31m" << "error : during creation" << "\033[0" << std::endl;
		return -2;
	}
	if (setsockopt(tempFd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
	{
		std::cout << "\033[31m" << "error : during settings" << "\033[0" << std::endl;
		return -1;
	}
	_socketFd = tempFd;
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	if (bind(_socketFd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		std::cout << "\033[31m" << "error : during binding IP" << "\033[0" << std::endl;
		return -1;
	}
	if (listen(_socketFd, BACKLOG) == -1)
	{
		std::cout << "\033[31m" << "error : during starting to listen" << "\033[0" << std::endl;
		return -1;
	}
	struct pollfd tmp;
	tmp.fd = tempFd;
	tmp.events = POLLIN;
	_fds.push_back(tmp);
	//std::cout << _fds[0].fd << std::endl;
	_timeout = (1000 * 60 * TIMEOUT_M + TIMEOUT_S * 1000);
	fcntl(this->_socketFd, 4, O_NONBLOCK);
	return (1);
}

int	Server::StartServer(){
	this->_ON = true;
	do
	{
		int	rp = poll(&this->_fds[0], this->_fds.size(), this->_timeout);
		if (rp < 0){
			std::cout << "Error: poll failed" << std::endl;
			break ;
		}
		else if (rp == 0){
			std::cout << "Error : poll() timed out." << std::endl;
			break ;
		}
		size_t	current_size = this->_fds.size();
		for (size_t i = 0; i < current_size; i++)
		{
			if (this->_fds[i].revents == 0)
				continue;
			if (this->_fds[i].revents != POLLIN){
				std::cout << "Error ! revents = " << this->_fds[i].revents << std::endl;
				this->_ON = false;
				break ;
			}
			if (this->_fds[i].fd == this->_socketFd)
			{
				std::cout << "Listening socket is readable" << std::endl;
				int new_sd = -1;
				do
				{
					new_sd = accept(this->_socketFd, NULL, NULL);
					if (new_sd < 0){
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Error: accept failed" << std::endl;
							this->_ON = false;
						}
						break ;
					}
					std::cout << "New incoming connection : " << new_sd << std::endl;
					struct pollfd	newpollfd;
					newpollfd.fd = new_sd;
					newpollfd.events = POLLIN;
					this->_fds.push_back(newpollfd);
				} while (new_sd != -1);
			}
			else
			{
				std::cout << "Descriptor is readable" << this->_fds[i].fd << std::endl;
				int	close_conn = false;
				char	buffer[2000];
				int	rc = -1;
				int	len = -1;
				do
				{
					rc = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Error: recv failed" << std::endl;
							close_conn = true;
						}
						break ;
					}
					if (rc == 0)
					{
						std::cout << "Connection closed" << std::endl;
						close_conn = true;
						break ;
					}
					len = rc;
					std::cout << len << " bytes received" <<std::endl;
					rc = send(this->_fds[i].fd, buffer, len, 0);
					if (rc < 0)
					{
						std::cout << "Error: send failed" << std::endl;
						close_conn = true;
						break ;
					}
				} while (true);
				if (close_conn)
				{
					close(this->_fds[i].fd);
					std::vector<struct pollfd>::iterator	it = this->_fds.begin();
					for (size_t z = 0; z < i; z++)
						it++;
					this->_fds.erase(it);
				}
			}
		}
		
	} while (this->_ON);
	for (size_t i = 0; i < this->_fds.size(); i++)
		close(this->_fds[i].fd);
	return 1;
}