#include"../includes/ft_irc.hpp"

#define IP INADDR_ANY
#define BACKLOG 3
#define BUFFER_LEN 200
#define NB_CLIENT 256

void init_sockaddr(struct sockaddr_in *addr, unsigned short port){
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = IP;
	addr->sin_port = htons(port);
}

int pars_port(t_data *dta, char **argv){
	std::string port(argv[1]);
	
	if (port.size() < 0 || port.size() > 5) {	
		std::cout << "error : Port range not valid" << std::endl; return 1;}
	dta->port = static_cast<unsigned short>(std::atoi(argv[1]));
	return (0);
}

int	init_socket(t_data *dta, struct sockaddr_in *addr) {
	int	number_option = 1;

	dta->fd_socket = -1;
	if ((dta->fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		std::cout << "Error : During socket creation" << std::endl; return (1);}
	if (setsockopt(dta->fd_socket, SOL_SOCKET, SO_REUSEADDR, &number_option, sizeof(number_option)) != 0){
		std::cout << "Error : During socket configuration" << std::endl; return (1);}
	if (bind(dta->fd_socket, (struct sockaddr *) addr, sizeof(* addr)) != 0){
		std::cout << "Error : During link IP/port to socket" << std::endl; return (1);}
	if (listen(dta->fd_socket, BACKLOG) != 0){
		std::cout << "Error : During listen" << std::endl; return (1);}
	fcntl(dta->fd_socket, F_SETFL, O_NONBLOCK);
	// if (connect(dta->fd_socket, (struct sockaddr *) addr, sizeof(* addr)) < 0){
		// std::cout << "Error : During connect" << std::endl; return (1);}
	std::cout << "Socket mis en ecoute" << std::endl;
	return (0);
}

// int	init_pollfd(struct pollfd *fds[2], t_data *dta) {

// 	fds[0]->fd = STDIN_FILENO;
// 	fds[0]->events = POLLIN | POLLOUT | POLLHUP | POLLERR | POLLNVAL;
// 	fds[1]->fd = dta->fd_socket;
// 	fds[1]->events = POLLIN | POLLOUT | POLLHUP | POLLERR | POLLNVAL;
// 	return (0);
// }

int waitForClient(int *fd_socket){
	int clientsocket;
	struct sockaddr_in clientAdrr;
	int clientsocketsize = sizeof(clientAdrr);

	if ((clientsocket = accept(*fd_socket, (struct sockaddr *)&clientAdrr, (socklen_t *)&clientsocketsize)) != -1) {
		char ip[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(clientAdrr.sin_addr), ip, INET_ADDRSTRLEN);
		std::cout << "New connection : " << ip << " " << clientAdrr.sin_port << std::endl;
		int opt = 1;
		setsockopt(clientsocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(1));
	}
	return (clientsocket);
}

void addClientToTab(int clientsocket, int client[]){
	int space = -1;

	for(int i = 0; i < NB_CLIENT; i++){
		if (client[i] == -1) {
			client[i] = clientsocket;
		}
		send(clientsocket, "Bienvenu zebi\n", strlen("Bienvenu zebi\n"),MSG_DONTWAIT);
		space = 0;
		break;
	}
	if (space == -1){
		std::cout << "No place left" << std::endl;
		close(clientsocket);
	}
}

void manageClient(int client[]){
	static char buf[BUFFER_LEN + 1];
	int clientsocket;

	for (int i = 0; i < NB_CLIENT; i++) {
		clientsocket = client[i];
		if (clientsocket == -1) {
			continue;
		}
		int len = recv(clientsocket, buf, BUFFER_LEN, MSG_DONTWAIT);
		int isclose = 0;
		if (len == -1 && errno != EAGAIN){
			std::cout << " Error : " << strerror(errno) << std::endl;
			isclose = 1;
		} else if (len == 0) {
			isclose = 1;
		} else if (len > 0) {
			buf[len] = '\0';
			if (strncmp(buf, "exit", 4) == 0) {
				send(clientsocket, "Bye\n", strlen("Bye\n"), 0);
				isclose = 1;
			}
			else {
				int len = strlen("Your msg : ") + strlen(buf) + 1;
				char rep[len];
				strcpy(rep, "Your msg : ");
				strcat(rep, buf);
				send(clientsocket, rep, strlen(rep), 0);
			}
		}
		if (isclose == 1){
			std::cout << "Client close connection" << std::endl;
			close(clientsocket);
			client[i] = -1;
		}
	}
}

int main(int argc, char **argv)
{
	int					clientsocket = 0;
	t_data				dta;
	struct sockaddr_in	adresse;
	struct pollfd		fds[NB_CLIENT];
	int					fdn = 1;
	int					end_server = 0;
	int					current_size = 1, i, j, len, compress_array, new_sd = -1, close_conn = -1;
	int					timeout = 3 * 60 * 1000;
	int 				client[NB_CLIENT];
	static char			buf[BUFFER_LEN + 1];

	for (int i = 0; i < NB_CLIENT; i++)
		client[i] = -1;
	if (argc != 3) {
		std::cout << "Usage : ./ircserv [port] [password]" << std::endl;
		return (1);
	}
	if (pars_port(&dta, argv)) return (1);
	init_sockaddr(&adresse, dta.port);
	if (init_socket(&dta, &adresse)) return (1);
	// init_pollfd(&fds, &dta);
	fds[0].fd = dta.fd_socket;
	fds[0].events = POLLIN;
	int ret = 0;
	do {
		// if ((clientsocket = waitForClient(&dta.fd_socket)) != -1)
			// addClientToTab(clientsocket, client);
		ret = poll(fds, fdn, timeout);
		if (ret < 0){
			std::cout << "Error : durring poll use" << std::endl; 
			return (1);
		} else if (ret == 0) {
			std::cout << "Error : poll() timed out.\n" << std::endl; 
			return (1);
		} else {
			current_size = fdn;
			for (i = 0; i < current_size; i++){
				if (fds[i].revents == 0)
					continue;
				if (fds[i].revents != POLLIN) {
					printf("Error! revents = %d\n", fds[i].revents);
					return -1;
				}
				if (fds[i].fd == dta.fd_socket) {
					std::cout << "Listening socket is readable" << std::endl;
					do {
						new_sd = accept(dta.fd_socket, NULL, NULL);
						if (new_sd < 0){
							if (errno != EWOULDBLOCK){
								std::cout << "Error : accept() failed" << std::endl;
								end_server = 1;
							}
							break;
						}
						std::cout << "New incoming connection - " << new_sd << std::endl;
						fds[fdn].fd = new_sd;
						fds[fdn].events = POLLIN;
						fdn++;
					} while (new_sd != -1);
				}
				else {
					std::cout << "Descriptor is readable : " << fds[i].fd << std::endl;
					close_conn = 0;
					do {
						ret = recv(fds[i].fd, buf, sizeof(buf), 0);
						if (ret < 0) {
							if (errno != EWOULDBLOCK){
								std::cout << "Error : recv() failed" << std::endl;
								close_conn = 1;
							}
							break;
						}
						if (ret == 0){
							std::cout << "Connection closed" << std::endl;
							close_conn = 1;
							break;
						}
						len = ret;
						std::cout << len << " byte received" << std::endl;
						ret = send(fds[i].fd, buf, len, 0);
						if (ret < 0) {
							std::cout << "Error : send() failed" << std::endl;
							close_conn = 1;
							break;
						}
					} while (1);
					if (close_conn = 1) {
						close(fds[i].fd);
						fds[i].fd = -1;
						compress_array = 1;
					}
				}
			}
			if (compress_array == 1){
				compress_array = 0;
				for (i = 0; i < fdn; i++){
					if (fds[i].fd == -1){
						for (j = i; j < fdn; j++){
							fds[j].fd = fds[j+1].fd;
						}
					i--;
					fdn--;
					}
				}
			}
			usleep(1000);
		}
		// manageClient(client);
	} while (end_server == 0);
	for (i = 0; i < fdn; i++){
		if (fds[i].fd >= -1){
			close(fds[i].fd);
		}
	}
	return (0);
}

// | SO_REUSEPORT