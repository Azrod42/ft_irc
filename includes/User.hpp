#ifndef USER_HPP_
# define USER_HPP_

#include"../includes/ft_irc.hpp"

class User {
	private :
		std::vector<t_user> _user;
		std::string			_pass;

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
		int				addUser(const unsigned int id, std::string name);
		void			disconectUser(unsigned int id);
		std::string 	getGestname(void) const;
		void			userCommand(char *prompt, unsigned int id);
		void			exeCommand(char *cmd_u, std::string full_cmd, unsigned int id);
		void			execLOG(std::string full_cmd, unsigned int id);
};

#endif
