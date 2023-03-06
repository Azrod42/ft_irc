#ifndef USER_HPP_
# define USER_HPP_

#include"../includes/ft_irc.hpp"

class User {
	private :
		std::vector<t_user> _user;
		std::string			_pass;

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
		void getServerPass(std::string pass){
			this->_pass = pass;
		}
		std::string	getName(unsigned int const id){
			std::vector<t_user>::iterator it = _user.begin();
			while (it++ != _user.end()){
				if (it->id == id){
					return (it->name);
				}
			}
			return("Warning: User not found");
		};
		std::string	getNick(unsigned int const id){
			std::vector<t_user>::iterator it = _user.begin();
			while (it++ != _user.end()){
				if (it->id == id){
					return (it->nick);
				}
			}
			return("Warning: User not found");
		};
		int				addUser(const unsigned int id, std::string name) {
			t_user udef;
			std::vector<t_user>::iterator it = _user.begin();

			if (name == "User")
				name = this->getGestname();
			while (it!= _user.end()){
				if (it->id == id || it->name == name){
					std::cout << "Error : addUser() not possible (ID or Name already use)."<<std::endl;
					return (1);
				}
				it++;
			}
			udef.id = id;
			udef.name = name;
			udef.nick = "undefine";
			udef.real = "undefine";
			udef.is_log = 0;
			udef.pass_ok = 0;
			_user.push_back(udef);
			std::cout << "New user connected : " << udef.name << std::endl;
			return (0);
		};
		std::string 	getGestname(void) const {
			std::string ret = "User_";
			srand(time(NULL));
			int nbr = rand() % 424242; 
			std::string s2(std::to_string(nbr));
			ret += s2;
			return (ret);
		};
		void			userCommand(std::string prompt, int id){
			char **cmds;
			int i = -1;

			cmds = ft_split(prompt.c_str(), '\n');
			while (cmds[++i]){
				std::cout << cmds[i] << std::endl;
				this->exeCommand(cmds[i], id);
			}
			// if (cmds);
				// free(cmds);
		};
		void			exeCommand(char *cmd_u, unsigned int id) {
			char **cmd = ft_split(cmd_u, ' ');
			std::vector<t_user>::iterator it = _user.begin();
			int j = 2;

			std::cout << "\n-------\n User " << id << " in exeCommand\n " << cmd_u << "\n----------" <<std::endl;
			if (std::string(cmd[0]) == "PASS")
				this->checkPASS(cmd[1], id);
			while (it != _user.end())
			{
				if (it->id == id){
					if (it->pass_ok == 1){
						if (std::string(cmd[0]) == "NICK")
							this->exeNICK(cmd[1], id);
						return ;
					}
				}
				it++;
			}
			if (j == 2)
				send(id, ERR_UNKNOWCOMMAND, sizeof(ERR_UNKNOWCOMMAND), 0);
			// else {std::cout << "nop" << std::endl;}

		};
		void			exeNICK(char *nick, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();

			for (int i = 0; nick[i]; i++){
				if (nick[i] == '\r' || nick[i] == '\n' || nick[i] == ' ' ||  nick[i] == '@'){
					send(id, "Erroneous Nickname\n", strlen("Erroneous Nickname\n"), 0);
					return ;
				}
			}
			if (std::string(nick).size() < 3){
				send(id, "Erroneous Nickname\n", strlen("Erroneous Nickname\n"), 0); return ;}
			while (it != _user.end()){
				if (it->id == id){
					it->nick = nick;
					return ;
				}
				it++;
			}	
		}
		void		checkPASS(char *pass, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();

			if (!pass){
				send(id, ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
				return;
			}
			while (it != _user.end())
			{
				if (it->id == id){
					if (it->pass_ok == 1){
						send(id, ERR_ALREADYREGISTRED, strlen(ERR_ALREADYREGISTRED), 0);
						return;
					}
				}
				it++;
			}
			if (std::string(pass).size() == 0){
				send(id, pass, strlen(pass), 0);
				send(id, ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
			}
			else if (std::string(pass) != this->_pass){
				send(id, ":invalid password\n", strlen(":invalid password\n"), 0);
			} else {
				it = _user.begin();
				while (it != _user.end())
				{
					if (it->id == id)
						it->pass_ok = 1;
					it++;
				}	
			}
		}
};

#endif