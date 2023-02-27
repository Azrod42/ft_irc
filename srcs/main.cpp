#include"../includes/ft_irc.hpp"

#define IP INADDR_ANY
#define BACKLOG 3

void init_sockaddr(struct sockaddr_in *addr, unsigned short port){
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = IP;
	addr->sin_port = htons(port);
}

int	init_socket(t_data *dta, struct sockaddr_in *addr) {
	int	number_option = 1;

	dta->fd_socket = -1;
	if ((dta->fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		std::cout << "Error : During socket creation" << std::endl; return (1);}
	if (setsockopt(dta->fd_socket, SOL_SOCKET, SO_REUSEADDR, &number_option, sizeof(number_option)) != 0){
		std::cout << "Error : During socket configuration" << std::endl; return (1);}
	if (bind(dta->fd_socket, (struct sockaddr *) addr, sizeof(addr)) != 0){
		std::cout << "Error : During dind on IP/port" << std::endl; return (1);}
	if (listen(dta->fd_socket, BACKLOG) != 0){
		std::cout << "Error : During listen" << std::endl; return (1);}
	fcntl(dta->fd_socket, F_SETFL, O_NONBLOCK);
	std::cout << "Socket mis en ecoute" << std::endl;
	return (0);
}

int pars_port(t_data *dta, char **argv){
	std::string port(argv[1]);
	
	if (port.size() < 0 || port.size() > 5) {	
		std::cout << "error : Port range not valid" << std::endl; return 1;}
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
	if (init_socket(&dta, &adresse)) return (1);
	// int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	// setsockopt(fd_socket, IPPROTO_TCP, SO_REUSEADDR, );
	// bind(fd_socket, const struct sockaddr *addr, socklen_t addrlen)


	std::cout << adresse.sin_port << std::endl;
	std::cout << argv[1] << " " << argv[2] << std::endl;
	return (0);
}
// | SO_REUSEPORT