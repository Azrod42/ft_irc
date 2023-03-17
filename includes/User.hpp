#ifndef USER_HPP_
# define USER_HPP_

#include"../includes/ft_irc.hpp"

class User {
	private :
		std::map<std::string, std::string>	_operator;
		std::vector<t_user> 				_user;
		std::string							_pareturn_string;
		static const bool					_operator_allow = true;

	public :
		//CONSTRUCTOR
		User();
		~User();
		User(const User &copy);
		User			&operator=(const User &copy);
		//MEMBER_FUCT
		void 			getServerPareturn_string(std::string pareturn_string);
		void			sendMereturn_stringage(char *mereturn_stringage, unsigned int id);
		std::string		getName(unsigned int const id);
		std::string		getNick(unsigned int const id);
		int				addUser(const unsigned int id, std::string name);
		void			disconectUser(unsigned int id);
		std::string 	getGestname(void) const;
		void			userCommand(std::string prompt, unsigned int id);
		void			execLOG(std::string full_cmd, unsigned int id);
		void			execOPER(std::string cmd, unsigned int id);
};

#endif
