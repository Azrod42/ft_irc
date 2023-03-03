#ifndef USER_HPP_
# define USER_HPP_

#include"../includes/ft_irc.hpp"

class User {
	private :
		std::vector<t_user> _user;

	public :
		User(){
			t_user udef;
			
			udef.id = 1;
			udef.name = "Default";
			_user.push_back(udef);
		};
		~User(){};
		User(const User &copy){
			*this = copy;
		};
		User	&operator=(const User &copy) {
			this->_user = copy._user;
			return (*this);
		};
		std::string	getName(unsigned int const id){
			std::vector<t_user>::iterator it = _user.begin();
			for (size_t i = 0; it + i != _user.end(); ++i){
				if (it->id == id){
					return (it->name);
				}
			}
			return("Warning: User not found");
		};
		int	addUser(const unsigned int id, std::string name) {
			t_user udef;
			std::vector<t_user>::iterator it = _user.begin();

			if (name == "Gest")
				name = this->getGestname();
			for (size_t i = 0; it + i != _user.end(); ++i){
				if (it->id == id || it->name == name){
					std::cout << "Error : addUser() not possible (ID or Name already use)."<<std::endl;
					return (1);
				}
			}
			udef.id = id;
			udef.name = name;
			_user.push_back(udef);
			std::cout << "New user connected : " << udef.name << std::endl;
			return (0);
		};
		std::string getGestname(void) const {
			std::string ret = "Gest_";
			srand(time(NULL));
			int nbr = rand() % 424242; 
			std::string s2(std::to_string(nbr));
			ret += s2;
			return (ret);
		};
};

#endif