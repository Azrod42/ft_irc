#include"../includes/User.hpp"

User::User(){
	t_user udef;
	udef.id = 1;
	udef.name = "Default";
	_user.push_back(udef);
};
User::~User(){};

User::User(const User &copy){
	*this = copy;
};

User	&User::operator=(const User &copy) {
	this->_user = copy._user;
	return (*this);
};

void User::getServerPass(std::string pass){
	this->_pass = pass;
}

void			User::sendMessage(char *message, unsigned int id){
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

std::string		User::getName(unsigned int const id){
	FINDUSER;

	if (it == _user.end())
		return("Warning: User not found");
	return (it->name);
};

std::string		User::getNick(unsigned int const id){
	std::vector<t_user>::iterator it = _user.begin();
	while (it++ != _user.end()){
		if (it->id == id){
			return (it->nick);
		}
	}
	return("Warning: User not found");
};

int				User::addUser(const unsigned int id, std::string name) {
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
	udef.nick = " ";
	udef.realname = " ";
	udef.is_log = 0;
	udef.pass_ok = 0;
	udef.mode = " ";
	udef.unused = " ";
	udef.cmd = strdup("");
	_user.push_back(udef);
	std::cout << "New user connected : " << udef.name << std::endl;
	return (0);
};

void			User::disconectUser(unsigned int id){
	std::vector<t_user>::iterator it = _user.begin();

	while (it != _user.end()){
	if (it->id == id){
		_user.erase(it);
		return ;
	}
	it++;
	}	
};

std::string 	User::getGestname(void) const {
	std::string ret = "User_";
	srand(time(NULL));
	int nbr = rand() % 424242; 
	std::string s2(std::to_string(nbr));
	ret += s2;
	return (ret);
};

void			User::userCommand(char *prompt, unsigned int id){
	std::vector<t_user>::iterator it = _user.begin();
	char **cmds;
	char *tmp;
	int i = -1;

	if (std::string(prompt).find("PASS ") < std::string::npos || std::string(prompt).find("NICK ") < std::string::npos || std::string(prompt).find("USER ") < std::string::npos){
		this->exeCommand(prompt, prompt, id);
		return;
}
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
					this->exeCommand(cmds[i], "", id);
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

void			User::exeCommand(char *cmd_u, std::string full_cmd, unsigned int id) {
	(void)cmd_u;
	std::vector<t_user>::iterator it = _user.begin();
	// std::cout << "\n-------\n User " << id << " in exeCommand\n " << cmd_u << "\n----------" <<std::endl;
	while (it != _user.end())
	{
		if (it->id == id){
			if (it->is_log > 3)
			{
				std::cout << "User enter CMD : " << cmd_u << std::endl;
				return ;
			}
			else
			{
				this->execLOG(full_cmd, id);
			}
		}
		it++;
	}
};

void	User::execLOG(std::string full_cmd, unsigned int id){
	FINDUSER
	
	//  std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\n"<< std::endl;
	if (full_cmd.find("PASS ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("PASS "), full_cmd.end());
		cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		if (cmd == this->_pass)
			it->pass_ok = 1;
		it->is_log += 1;
	}
	if (full_cmd.find("NICK ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("NICK "), full_cmd.end());
		cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		//NO_NICK_GIVEN
		if (cmd.find("\r\n") == 0){
			std::string rep = error_nonicknamegiven();
			send(id, rep.c_str(), rep.size(), 0);
			return;
		}
		//NICK_ALREADY_USE
		std::vector<t_user>::iterator itc = _user.begin();
		while (itc != _user.end())
		{
			if (itc->nick == cmd)
			{
				std::string rep = error_nicknameinuse(cmd);
				send(id, rep.c_str(), rep.size(), 0);
				return;
			}
			itc++;
		}
		//CHECK_VALID_NICKNAME
		if (check_nickname(cmd)) {
			std::string rep = error_erroneusnickname(cmd);
			send(id, rep.c_str(), rep.size(), 0);
			return;
		}
		it->nick = cmd;
		it->is_log += 1;
	}
	if (full_cmd.find("USER ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("USER "), full_cmd.end());
		cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);	
		char **cmds = ft_split(cmd.c_str(), ' ');
		int i = -1; while (cmds[++i]);
		//NEED_MORE_PARAM
		if (i < 3) {
			std::string rep = error_needmoreparams("NICK");
			send(id, rep.c_str(), rep.size(), 0);
			return ;
		}
		//ALREADY_REGISTER
		if (it->realname != " " || it->is_log == 1){
			std::string rep = error_alreadyregistred();
			send(id, rep.c_str(), rep.size(), 0);
			return ;
		}
		//SET_VAL
		it->name = cmds[0]; free(cmds[0]);
		it->mode = cmds[1]; free(cmds[1]);
		it->unused = cmds[2]; free(cmds[2]);
		for (int i = 3; cmds[i]; i++){
			if (i == 3){
				it->realname = &cmds[i][1]; free(cmds[i]);}
			else {
				it->realname = cmds[i]; free(cmds[i]);}
		}
		free(cmds);
		it->is_log += 1;
	}
	if (it->name != " " && it->nick != " " && it->pass_ok == 1) {
		it->is_log = 4;
		std::string rep = rplwelcome(it->nick, it->name);
		send(id, rep.c_str(), rep.size(), 0);
	}
	std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\n"<< std::endl;
};

