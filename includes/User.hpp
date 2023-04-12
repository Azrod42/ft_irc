#ifndef USER_HPP_
# define USER_HPP_

#include"../includes/ft_irc.hpp"

class User {
	private :
		std::map<std::string, std::string>	_operator;
		std::vector<t_user> 				_user;
		Channel								_channel[NUMBER_CHANNEL_MAX];
		int									_channel_use;
		std::string							_pass;
		static const bool					_operator_allow = true;

	public :
		//CONSTRUCTOR
		User();
		~User();
		User(const User &copy);
		User			&operator=(const User &copy);
		//MEMBER_FUCT
		void 			getServerPass(std::string pass);
		void			sendMessage(char *message, unsigned int id);
		std::string		getName(unsigned int const id);
		std::string		getNick(unsigned int const id);
		int				addUser(const unsigned int id, std::string name, in_addr_t ip);
		void			disconectUser(unsigned int id);
		std::string 	getGestname(void) const;
		unsigned int	userCommand(std::string prompt, unsigned int id);
		void			execLOG(std::string full_cmd, unsigned int id);
		void			execKICK(std::string cmd, unsigned int id);
		unsigned int	execKILL(std::string cmd, unsigned int id);
		unsigned int	execDIE(std::string cmd, unsigned int id);
		void			execMODE(std::string cmd, unsigned int id);
		void			execOPER(std::string cmd, unsigned int id);
		void			execPING(std::string cmd, unsigned int id);
		void			execJOIN(std::string cmd, unsigned int id);
		void			execPART(std::string cmd, unsigned int id);
		void			execPRIVMSGC(std::string cmd, unsigned int id);
		void			execPRIVMSGU(std::string cmd, unsigned int id);
		void			execMODEO(std::vector<t_user>::iterator it, std::vector<t_user>::iterator it2, Channel *channel);
};

#endif
