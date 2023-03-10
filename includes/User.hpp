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

		void		sendMessage(char *message, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();
			std::string user = "";
			
			while (it++ != _user.end()){
				if (it->id == id){
					user = it->nick + " ";
				}
			}
			user += std::string(message);
			send(id, user.c_str(), sizeof(user.c_str()), 0);
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
			udef.nick = "";
			udef.realname = "";
			udef.is_log = 0;
			udef.pass_ok = 0;
			udef.mode = "";
			udef.unused = "";
			udef.cmd = strdup("");
			_user.push_back(udef);
			std::cout << "New user connected : " << udef.name << std::endl;
			return (0);
		};

		void	disconectUser(unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();

			while (it != _user.end()){
			if (it->id == id){
				_user.erase(it);
				return ;
			}
			it++;
			}	
		};

		std::string 	getGestname(void) const {
			std::string ret = "User_";
			srand(time(NULL));
			int nbr = rand() % 424242; 
			std::string s2(std::to_string(nbr));
			ret += s2;
			return (ret);
		};

		void			userCommand(char *prompt, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();
			char **cmds;
			char *tmp;
			int i = -1;

			while (it != _user.end())
			{
				if (it->id == id)
				{
					tmp = it->cmd;
					it->cmd = ft_strjoin(it->cmd, prompt);
					free(tmp);
					if (strchr(it->cmd, '\n') != NULL)
					{
						cmds = ft_split(it ->cmd, '\n');
						while (cmds[++i]){
							this->exeCommand(cmds[i], id);
						}
						free(it->cmd);
						it->cmd = strdup("");
					}
				}
				it++;
			}
			// if (cmds);
				// free(cmds);
			free(prompt);
		};

		void			userCommandfull(char *prompt, int id){
			char **cmds = ft_split(prompt, '\n');
			int i = -1;

			while (cmds[++i]){
				this->exeCommand(cmds[i], id);
			}
			// if (cmds);
				// free(cmds);
			free(prompt);
		};

		void			exeCommand(char *cmd_u, unsigned int id) {
			char **cmd = ft_split(cmd_u, ' ');
			std::vector<t_user>::iterator it = _user.begin();

			std::cout << "\n-------\n User " << id << " in exeCommand\n " << cmd_u << "\n----------" <<std::endl;
			if (std::string(cmd[0]) == "PASS")
				this->checkPASS(cmd[1], id);
			while (it != _user.end())
			{
				if (it->id == id){
					if (it->pass_ok == 1){
						if (std::string(cmd[0]) == "PASS")
							return ;
						if (it->is_log == 2) {
							if (std::string(cmd[0]) == "cmd")
								(void)id;//commande a exex
							else
								send(id, ERR_UNKNOWCOMMAND, sizeof(ERR_UNKNOWCOMMAND), 0);
						}
						else {
							if (std::string(cmd[0]) == "NICK" && cmd[1])
								this->exeNICK(cmd[1], id);
							else if (std::string(cmd[0]) == "USER")
								this->checkUSER(&cmd[1], id);
							else
								send(id, ":User not login\n", sizeof(":User not login\n"), 0);	
						}
						return ;
					}
					else if (std::string(cmd[0]) != "PASS" || std::string(cmd[0]) != "CAP"){
							send(id, ":Please enter PASS\n", sizeof(":Please enter PASS\n"), 0);
					}
				}
				it++;
			}
		};

		void			exeNICK(char *nick, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();

			for (int i = 0; nick[i]; i++){
				if (nick[i] == '\r' || nick[i] == '\n' || nick[i] == ' ' ||  nick[i] == '@'){
					send(id, "Erroneous Nickname\n", strlen("Erroneous Nickname\n"), 432);
					return ;
				}
			}
			if (std::string(nick).size() < 3){
				send(id, "Erroneous Nickname\n", strlen("Erroneous Nickname\n"), 432); return ;}
			while (it != _user.end()){
				if (it->id == id){
					it->nick = nick;
					it->is_log += 1;
					return ;
				}
				it++;
			}	
		};

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
					{
						it->pass_ok = 1;
					}
					it++;
				}	
			}
		};

		void		checkUSER(char **cmd, unsigned int id){
			std::vector<t_user>::iterator it = _user.begin();
			int i = -1;

			if (!cmd){
				send(id, ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
				return;
			}
			while (cmd[++i])
				;
			if (i < 3){
				send(id, ERR_NEEDMOREPARAMS, strlen(ERR_NEEDMOREPARAMS), 0);
				return;
			}
			while (it != _user.end()){
				if (it->id == id){
					it->name = cmd[0];
					it->mode = cmd[1];
					it->unused = cmd[2];
					for (int i = 3; cmd[i]; i++){
						if (i == 3)
							it->realname += &cmd[i][1];
						else
							it->realname += cmd[i];
						it->realname += " ";
					}
					it->is_log += 1;
					// std::cout << "User id = " << id << " name : " << it->name << " mode : " << it->mode << " unused : " << it->unused << " realname : " << it->realname << std::endl; 
					return ;
				}
				it++;
			}	
		};
};

#endif