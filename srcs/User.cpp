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
	udef.pass_ok = false;
	udef.nick_ok = false;
	udef.user_ok = false;
	udef.mode = " ";
	udef.unused = " ";
	udef.cmd = "";
	_user.push_back(udef);
	std::cout << "New user connected : " << udef.name << std::endl;
	return (0);
};

void			User::disconectUser(unsigned int id){
	FINDUSER
	_user.erase(it);
};

std::string 	User::getGestname(void) const {
	std::string ret = "User_";
	srand(time(NULL));
	int nbr = rand() % 424242; 
	std::string s2(std::to_string(nbr));
	ret += s2;
	return (ret);
};

void			User::userCommand(std::string prompt, unsigned int id){
	FINDUSER
	
	it->cmd += prompt;
	if (it->cmd.find("\n") < std::string::npos && it->cmd.find("\r") == std::string::npos)
		it->cmd.insert(it->cmd.find("\n"), "\r");
	if (it->cmd.find("\r\n") < std::string::npos)
	{
		if (it->is_log == 4)
		{
			std::cout << "User enter CMD : " << it->cmd << std::endl;
		}
		else
		{
			this->execLOG(it->cmd, id);
		}
		it->cmd = "";
	}	
}

void	User::execLOG(std::string full_cmd, unsigned int id){
	FINDUSER
	
	//  std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\n"<< std::endl;
	if (full_cmd.find("PASS ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("PASS "), full_cmd.end());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		//ALREADY_REGISTER
		if (it->pass_ok == 1){
			std::string rep = error_alreadyregistred();
			send(id, rep.c_str(), rep.size(), 0);
		}
		//NEED_MORE_PARAM
		if (!check_empty(cmd)){
			std::string rep = error_needmoreparams("PASS");
			send(id, rep.c_str(), rep.size(), 0);
			return;
		}
		if (cmd == this->_pass)
			it->pass_ok = true;
	}
	if (full_cmd.find("NICK ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("NICK "), full_cmd.end());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		//NO_NICK_GIVEN
		if (!check_empty(cmd)){
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
		it->nick_ok = true;
	}
	if (full_cmd.find("USER ") < std::string::npos){
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("USER "), full_cmd.end());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);	
		char **cmds = ft_split(cmd.c_str(), ' ');
		int i = -1; while (cmds[++i]);
		//NEED_MORE_PARAM
		if (i < 4) {
			std::string rep = error_needmoreparams("NICK");
			send(id, rep.c_str(), rep.size(), 0);
			return ;
		}
		//ALREADY_REGISTER
		if (it->user_ok == true) {
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
				it->realname += cmds[i]; free(cmds[i]);}
			it->realname += " ";
		}
		free(cmds);
		it->user_ok = true;
	}
	if (it->nick_ok == true && it->pass_ok == true && it->user_ok == true) {
		it->is_log = 4;
		std::string rep = rplwelcome(it->nick, it->name);
		send(id, rep.c_str(), rep.size(), 0);
	}
	std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\nUSER :" << it->name << "\nREAL_NAME :" << it->realname << "\n"<< std::endl;
};

