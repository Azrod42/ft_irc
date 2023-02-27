#include"../includes/ft_irc.hpp"

#define IP INADDR_ANY

void init_sockaddr(struct sockaddr_in *addr, unsigned short port){
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = IP;
	addr->sin_port = htons(port);
}

int pars_port(t_data *dta, char **argv){
	std::string port(argv[1]);
	
	if (port.size() < 0 || port.size() > 5) {	
		std::cout << "error : Invalid port" << std::endl; return 1;}
	dta->port = static_cast<unsigned short>(std::atoi(argv[1]));
	return (0);
}

int main(int argc, char **argv)
{
	t_data dta;
	struct sockaddr_in adresse;

	if (argc != 3) {
		std::cout << "Usage : ./ircserv [port] [password]" << std::endl;
		return (1);
	}
	if (pars_port(&dta, argv)) return (1);
	init_sockaddr(&adresse, dta.port);
	// int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	// setsockopt(fd_socket, IPPROTO_TCP, SO_REUSEADDR, );
	// bind(fd_socket, const struct sockaddr *addr, socklen_t addrlen)


	std::cout << adresse.sin_port << std::endl;
	std::cout << argv[1] << " " << argv[2] << std::endl;
	return (0);
}