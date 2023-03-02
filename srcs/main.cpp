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
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN | POLLOUT | POLLHUP | POLLERR | POLLNVAL;
	fds[1].fd = dta.fd_socket;
	fds[1].events = POLLIN | POLLOUT | POLLHUP | POLLERR | POLLNVAL;
	int ret = 0;
	while (1){
		if ((clientsocket = waitForClient(&dta.fd_socket)) != -1)
			addClientToTab(clientsocket, client);
		ret = poll(fds, 2, -1);
		if (ret < 0){
			std::cout << "Error : durring poll use" << std::endl; 
			return (1);
		}
		if (ret == -1) {
		} else if (ret == 1000) {
			printf("timeout expired\n");
		} else {
			if (fds[0].revents & POLLIN) {
				int len = recv(dta.fd_socket, buf, BUFFER_LEN, MSG_DONTWAIT);
				buf[len] = '\0';
				std::cout << "Recive 1 : " << buf << std::endl;
				// printf("stdin is ready to be read\n");
			}
			if (fds[1].revents & POLLIN) {
				int len = recv(dta.fd_socket, buf, BUFFER_LEN, MSG_DONTWAIT);
				buf[len] = '\0';
				std::cout << "Recive 2 : " << buf << std::endl;
				// printf("socket is ready to be read\n");
			}
			if (fds[1].revents & POLLOUT) {
				int len = recv(dta.fd_socket, buf, BUFFER_LEN, MSG_DONTWAIT);
				buf[len] = '\0';
				std::cout << "Recive 3 : " << buf << std::endl;
				printf("socket is ready to be written\n");
			}
			usleep(1000);
		}
		// manageClient(client);
	// }
	return (0);
}

// | SO_REUSEPORT