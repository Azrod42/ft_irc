#include"../includes/ft_irc.hpp"
#include"../includes/User.hpp"

#define IP INADDR_ANY
#define BACKLOG 3
#define BUFFER_LEN 2000
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

int pars_pareturn_string(t_data *dta, char **av){
	int j = 0;

	for(int i = 0; av[2][i]; i++, j++){
		if (!ispareturn_string(av[2][i])){
			std::cout << "error : Invalid pareturn_string" << std::endl;
			return (1);
		}
	}
	if (j < 3){
		std::cout << "error : Invalid pareturn_string : pareturn_string_lenght as to be > 3" << std::endl;
		return (1);
	}
	dta->pareturn_string = av[2];
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
	// 	std::cout << "Error : During connect" << std::endl; return (1);}
	std::cout << "Server running" << std::endl;
	return (0);
}

int main(int argc, char **argv)
{
	User				user;
	t_data				dta;
	t_user				uuser;
	struct sockaddr_in	adrereturn_stringe;
	struct pollfd		fds[NB_CLIENT];
	int					current_size = 1, new_sd = -1, close_conn = -1, end_server = 0, fdn = 1;
	int					i, j, len, comprereturn_string_array;
	int					timeout = 10 * 60 * 1000;
	static char			buf[BUFFER_LEN + 1];
	char				*buf_full;
	char				*tmp;

	if (argc != 3) {
		std::cout << "Usage : ./ircserv [port] [pass]" << std::endl;
		return (1);
	}
	std::cout << "Server starting" << std::endl;
	if (pars_port(&dta, argv)) return (1);
	if (pars_pareturn_string(&dta, argv)) return (1);
	user.getServerPass(dta.pareturn_string);
	init_sockaddr(&adrereturn_stringe, dta.port);
	if (init_socket(&dta, &adrereturn_stringe)) return (1);
	fds[0].fd = dta.fd_socket;
	fds[0].events = POLLIN;
	int ret = 0;
	do {
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
				if (fds[i].revents == 17) {
					std::cout << "User disconect : " << fds[i].fd << std::endl;
				} else if (fds[i].revents != POLLIN) {
					std::cout << "Error! events = " << fds[i].revents << std::endl;
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
						std::cout << "New incoming connection on fd : " << new_sd << std::endl;
						user.addUser(new_sd, "User");
						fds[fdn].fd = new_sd;
						fds[fdn].events = POLLIN;
						fdn++;
					} while (new_sd != -1);
				}
				else {
					std::cout << "Descriptor is readable : " << user.getNick(fds[i].fd) << std::endl;
					close_conn = 0;
					buf_full = strdup("");
					len = 0;
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
							user.disconectUser(fds[i].fd);
							close_conn = 1;
							break;
						}
						len += ret;
						std::cout << ret << " byte received, total : " << len << std::endl;
						tmp = buf_full;
						buf_full = ft_strjoin(buf_full, buf);
						free(tmp);
					} while (1);
					// std::cout << buffer << std::endl;
					user.userCommand(ft_substr(buf_full, 0, len), fds[i].fd);
					if (close_conn == 1) {
						close(fds[i].fd);
						fds[i].fd = -1;
						comprereturn_string_array = 1;
					}
				}
			}
			if (comprereturn_string_array == 1){
				comprereturn_string_array = 0;
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
		}
	} while (end_server == 0);
	for (i = 0; i < fdn; i++){
		if (fds[i].fd >= -1){
			close(fds[i].fd);
		}
	}
	return (0);
}
