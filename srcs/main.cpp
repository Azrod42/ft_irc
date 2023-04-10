#include"../includes/ft_irc.hpp"
#include"../includes/User.hpp"
#include"../includes/Server.hpp"



int	pars_port(char* port)
{
	std::string	p;
	p += port;
	for (size_t i = 0; i < p.length(); i++){
		if (!std::isdigit(p[i]))
		{
			std::cout << "error: '" << p[i] << "' in " + p << " is not a number" << std::endl;
			return (0);
		}
	}
	if (p.length() < 1)
	{
		std::cout << "error: why do you try this ?" << std::endl;
		return (0);
	}
	if (p.length() > 5)
	{
		std::cout << "error: port too long" << std::endl;
		return (0);
	}
	int	nb = std::stoi(p);
	if (nb > MAXPORT)
	{
		std::cout << "error: port too big. (1 <= port <= " << MAXPORT << ")" << std::endl;
		return (0);
	}
	return (1);
}

int	pars_password(char* pass)
{
	std::string p;
	p += pass;
	for (size_t i = 0; i < p.length(); i++){
		if (!std::isalnum(p[i]))
		{
			std::cout << "error: '" << p << "' is not a valid password" << std::endl;
			return (0);
		}
	}
	if (p.length() < 1)
	{
		std::cout << "\033[31mError:\033[0m why do you try this ?" << std::endl;
		return (0);
	}
	return (1);
}

int	pars_hub(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	if (!pars_port(av[1]))
		return 0;
	if (!pars_password(av[2]))
		return 0;
	return 1;
}

int	main(int ac, char **av)
{
	if (!pars_hub(ac, av))
		return (1);
	//t_system	sys;
	Server		server(av[1], av[2]);
	int	tmp6 = server.CreateServer();
	if (tmp6 < 0)
	{
		if (tmp6 == -1)
			close(server.getServerFD());
		return -1;
	}
	server.StartServer();
	close(server.getServerFD());
	return (0);
}



// int	init_socket(t_data *dta, struct sockaddr_in *addr) {
// 	int	number_option = 1;

// 	dta->fd_socket = -1;
// 	if ((dta->fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
// 		std::cout << "Error : During socket creation" << std::endl; return (1);}
// 	if (setsockopt(dta->fd_socket, SOL_SOCKET, SO_REUSEADDR, &number_option, sizeof(number_option)) != 0){
// 		std::cout << "Error : During socket configuration" << std::endl; return (1);}
// 	if (bind(dta->fd_socket, (struct sockaddr *) addr, sizeof(* addr)) != 0){
// 		std::cout << "Error : During link IP/port to socket" << std::endl; return (1);}
// 	if (listen(dta->fd_socket, BACKLOG) != 0){
// 		std::cout << "Error : During listen" << std::endl; return (1);}
// 	fcntl(dta->fd_socket, F_SETFL, O_NONBLOCK);
// 	std::cout << "Server running" << std::endl;
// 	return (0);
// }

// int main(int argc, char **argv)
// {
// 	User				user;
// 	t_data				dta;
// 	t_user				uuser;
// 	struct sockaddr_in	adresse;
// 	struct pollfd		fds[NB_CLIENT];
// 	int					current_size = 1, new_sd = -1, close_conn = -1, end_server = 0, fdn = 1;
// 	int					i, j, len, compress_array;
// 	int					timeout = 1 * 60 * 1000;
// 	static char			buf[BUFFER_LEN + 1];
// 	char				*buf_full;
// 	char				*tmp;

// 	if (argc != 3) {
// 		std::cout << "Usage : ./ircserv [port] [pass]" << std::endl;
// 		return (1);
// 	}
// 	std::cout << "Server starting" << std::endl;
// 	if (pars_port(&dta, argv)) return (1);
// 	if (pars_pareturn_string(&dta, argv)) return (1);
// 	user.getServerPass(dta.pareturn_string);
// 	init_sockaddr(&adresse, dta.port);
// 	if (init_socket(&dta, &adresse)) return (1);
// 	fds[0].fd = dta.fd_socket;
// 	fds[0].events = POLLIN;
// 	int ret = 0;
// 	do {
// 		ret = poll(fds, fdn, timeout);
// 		if (ret < 0){
// 			std::cout << "Error : durring poll use" << std::endl; 
// 			return (1);
// 		} else if (ret == 0) {
// 			std::cout << "Error : poll() timed out.\n" << std::endl; 
// 			return (1);
// 		} else {
// 			current_size = fdn;
// 			for (i = 0; i < current_size; i++){
// 				if (fds[i].revents == 0)
// 					continue;
// 				if (fds[i].revents == 17) {
// 					std::cout << "User disconect" << std::endl;// : " << fds[i].fd << std::endl;
// 				} else if (fds[i].revents != POLLIN && fds[i].revents != 32) {
// 					std::cout << "Error! events = " << fds[i].revents << std::endl;
// 					perror(strerror(fds[i].revents));
// 					return -1;
// 				}
// 				if (fds[i].fd == dta.fd_socket && fds[i].revents != 32) {
// 					std::cout << "Listening socket is readable" << std::endl;
// 					do {
// 						new_sd = accept(dta.fd_socket, NULL, NULL);
// 						if (new_sd < 0){
// 							if (errno != EWOULDBLOCK){
// 								std::cout << "Error : accept() failed" << std::endl;
// 								end_server = 1;
// 							}
// 							break;
// 						}
// 						std::cout << "New incoming connection" << std::endl;//on fd : " << new_sd << std::endl;
// 						user.addUser(new_sd, "User", adresse.sin_addr.s_addr);
// 						fds[fdn].fd = new_sd;
// 						fds[fdn].events = POLLIN;
// 						fdn++;
// 					} while (new_sd != -1);
// 				}
// 				else if (fds[i].revents != 32){
// 					// std::cout << "Descriptor is readable : " << user.getNick(fds[i].fd) << std::endl;
// 					close_conn = 0;
// 					buf_full = strdup("");
// 					len = 0;
// 					do {
// 						ret = recv(fds[i].fd, buf, sizeof(buf), 0);
// 						if (ret < 0) {
// 							if (errno != EWOULDBLOCK){
// 								std::cout << "Error : recv() failed" << std::endl;
// 								close_conn = 1;
// 							}
// 							break;
// 						}
// 						if (ret == 0){
// 							std::cout << "Connection closed" << std::endl;
// 							user.disconectUser(fds[i].fd);
// 							close_conn = 1;
// 							break;
// 						}
// 						len += ret;
// 						std::cout << ret << " byte received" << std::endl;
// 						tmp = buf_full;
// 						buf_full = ft_strjoin(buf_full, buf);
// 						free(tmp);
// 					} while (1);
// 					// std::cout << buffer << std::endl;
// 					tmp = ft_substr(buf_full, 0, len); 
// 					unsigned int ret = user.userCommand(tmp, fds[i].fd);
// 					free(tmp);
// 					free(buf_full);
// 					if (ret == 1000){
// 						end_server = 1;
// 					} else if (ret != 999){
// 						int p = 0;
// 						while (p < NB_CLIENT) {
// 							if (fds[p].fd == (int)ret)
// 								break;
// 							p++;
// 						}
// 						if (p != 256) {
// 						close(fds[p].fd);
// 						fds[p].fd = -1;
// 						compress_array = 1;
// 						}
// 					}
// 					if (close_conn == 1) {
// 						close(fds[i].fd);
// 						fds[i].fd = -1;
// 						compress_array = 1;
// 					}
// 				}
// 			}
// 			if (compress_array == 1){
// 				compress_array = 0;
// 				for (i = 0; i < fdn; i++){
// 					if (fds[i].fd == -1){
// 						for (j = i; j < fdn; j++){
// 							fds[j].fd = fds[j+1].fd;
// 						}
// 					i--;
// 					fdn--;
// 					}
// 				}
// 			}
// 		}
// 	} while (end_server == 0);
// 	for (i = 0; i < fdn; i++){
// 		if (fds[i].fd >= -1){
// 			close(fds[i].fd);
// 		}
// 	}
// 	// system("leaks ircserv");
// 	return (0);
// }
