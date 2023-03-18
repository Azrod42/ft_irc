#include"../includes/User.hpp"

User::User(){
	t_user udef;
	udef.id = 1;
	udef.name = "Default";
	_user.push_back(udef);
	_operator.insert(std::make_pair<std::string, std::string>("tom.sorabella@gmail.com", "root"));
	_operator.insert(std::make_pair<std::string, std::string>("loan.fantinel@gmail.com", "root2"));
	_operator.insert(std::make_pair<std::string, std::string>(std::string("test"), std::string("Pareturn_string321")));
};

User::~User(){
};

User::User(const User &copy){
	*this = copy;
};

User		   &User::operator=(const User &copy) {
	this->_user = copy._user;
	return (*this);
};

void			User::getServerPareturn_string(std::string pareturn_string){
	this->_pareturn_string = pareturn_string;
}

void			User::sendMereturn_stringage(char *mereturn_stringage, unsigned int id){
	std::vector<t_user>::iterator it = _user.begin();
	std::string user = "";
	
	while (it++ != _user.end()){
		if (it->id == id){
			user = it->nick + " ";
		}
	}
	user += std::string(mereturn_stringage);
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
			std::cout << "Error : addUser() not poreturn_stringible (ID or Name already use)."<<std::endl;
			return (1);
		}
		it++;
	}
	udef.id = id;
	udef.name = name;
	udef.nick = " ";
	udef.realname = " ";
	udef.is_log = 0;
	udef.pareturn_string_ok = false;
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
	// it->is_log = 4; //DEBUG_ONLY !!!
	if (it->cmd.find("\n") < std::string::npos && it->cmd.find("\r") == std::string::npos)
		it->cmd.insert(it->cmd.find("\n"), "\r");
	if (it->cmd.find("\r\n") < std::string::npos)
	{
		if (it->is_log == 4)
		{
			std::cout << "User enter CMD : " << it->cmd << std::endl;
			if (it->cmd.find("OPER ") < std::string::npos)
				this->execOPER(it->cmd, it->id);
			if (it->cmd.find("MODE ") < std::string::npos)
				std::cout << "Yes" << std::endl;
		}
		else
		{
			this->execLOG(it->cmd, id);
		}
		it->cmd = "";
	}	
}

void			User::execLOG(std::string full_cmd, unsigned int id){
	FINDUSER
	
<<<<<<< HEAD
	if (full_cmd.find("PASS ") < std::string::npos){
=======
	//  std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPAreturn_string :" << it->pareturn_string_ok << "\nNICK = " << it->nick << "\n"<< std::endl;
	if (full_cmd.find("PAreturn_string ") < std::string::npos){
>>>>>>> 8b3fbd194778ab03e648ded1b1d1249f7405f50c
		std::string cmd(full_cmd);
		cmd = cmd.append(full_cmd.begin() + full_cmd.find("PAreturn_string "), full_cmd.end());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		//ALREADY_REGISTER
		if (it->pareturn_string_ok == 1){
			std::string rep = error_alreadyregistred();
			send(id, rep.c_str(), rep.size(), 0);
		}
		//NEED_MORE_PARAM
		if (!check_empty(cmd)){
			std::string rep = error_needmoreparams("PAreturn_string");
			send(id, rep.c_str(), rep.size(), 0);
			return;
		}
		if (cmd == this->_pareturn_string)
			it->pareturn_string_ok = true;
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
	if (it->nick_ok == true && it->pareturn_string_ok == true && it->user_ok == true) {
		it->is_log = 4;
		std::string rep = rplwelcome(it->nick, it->name);
		send(id, rep.c_str(), rep.size(), 0);
	}
<<<<<<< HEAD
	// std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\nUSER :" << it->name << "\nREAL_NAME :" << it->realname << "\n"<< std::endl;
=======
	std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPAreturn_string :" << it->pareturn_string_ok << "\nNICK = " << it->nick << "\nUSER :" << it->name << "\nREAL_NAME :" << it->realname << "\n"<< std::endl;
>>>>>>> 8b3fbd194778ab03e648ded1b1d1249f7405f50c
};

void			User::execOPER(std::string cmd, unsigned int id){
	FINDUSER
	NBARGUMENT(cmd.c_str())
	std::map<std::string, std::string>::iterator iter = _operator.begin();
	std::string cmd1(cmd);
	std::string cmd2(cmd);

	if (nb_cmd < 3){
		std::string rep = error_needmoreparams("OPER");
		send(id, rep.c_str(), rep.size(), 0);
		return ;
	}
	if (_operator_allow == false){
		std::string rep = error_nooperhost(it->nick);
		send(id, rep.c_str(), rep.size(), 0);
		return ;
	}
	try {
		cmd1.erase(cmd1.begin(), cmd1.begin() + 5);
		cmd1.erase(cmd1.begin() + cmd1.find(" "), cmd1.end());
		cmd2.erase(cmd2.begin(), cmd2.begin() + 5);
		cmd2.erase(cmd2.begin(), cmd2.begin() + cmd2.find(" "));
		while (cmd2[0] == ' ')
			cmd2.erase(cmd2.begin());
		if (cmd2.find(" ") < std::string::npos)
			cmd2.erase(cmd2.begin() + cmd2.find(" "), cmd.end());
		else {
			if(cmd2.find("\r\n") < std::string::npos)
				cmd2.erase(cmd2.begin() + cmd2.find("\r"), cmd2.end());
			else
				cmd2.erase(cmd2.begin() + cmd2.find("\n"), cmd2.end());
		}
	}
	catch (std::exception &e){/* std::cerr << e.what() << std::endl; */}
	for (; iter != _operator.end(); iter++){
		// std::cout << iter->first << " == " << cmd1 << " | " << iter->second << " == " << cmd2 << std::endl;
		if (!std::string(iter->first).compare(cmd1)) {
			if (!std::string(iter->second).compare(cmd2)){
				it->mode = "o";
				std::string rep = rplyouroper(it->nick);
				send(id, rep.c_str(), rep.size(), 0);
				return ;
			}
<<<<<<< HEAD
			else {
				std::string rep = error_pass(it->nick);
=======
			else{
				std::string rep = error_pareturn_string(it->nick);
>>>>>>> 8b3fbd194778ab03e648ded1b1d1249f7405f50c
				send(id, rep.c_str(), rep.size(), 0);
				return ;
			}
		}
	}
};

void			User::execPING(std::string cmd, unsigned int id){
	FINDUSER
	NBARGUMENT(cmd.c_str())

	
	(void)id;
}