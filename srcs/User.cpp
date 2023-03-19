#include"../includes/User.hpp"

User::User() : _channel_use(0){
	t_user udef;
	udef.id = 1;
	udef.name = "Default";
	_user.push_back(udef);
	_operator.insert(std::make_pair<std::string, std::string>("tom.sorabella@gmail.com", "root"));
	_operator.insert(std::make_pair<std::string, std::string>("loan.fantinel@gmail.com", "root2"));
	_operator.insert(std::make_pair<std::string, std::string>(std::string("test"), std::string("root")));
};

User::~User(){
};

User::User(const User &copy){
	*this = copy;
};

User		   &User::operator=(const User &copy) {
	this->_user = copy._user;
	this->_channel_use = copy._channel_use;
	return (*this);
};

void			User::getServerPass(std::string pass){
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
			std::cout << "Error : addUser() not possible (ID or Name already use)." << std::endl;
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
	udef.is_operator = false;
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
			if (it->cmd.find("PING ") < std::string::npos)
				this->execPING(it->cmd, it->id);
			if (it->cmd.find("JOIN ") < std::string::npos)
				this->execJOIN(it->cmd, it->id);
		}
		else
			this->execLOG(it->cmd, id);
		it->cmd = "";
	}	
}

void			User::execLOG(std::string full_cmd, unsigned int id){
	FINDUSER
	
	//std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pareturn_string_ok << "\nNICK = " << it->nick << "\n"<< full_cmd << std::endl;
	if (full_cmd.find("PASS ") < std::string::npos){
		std::string cmd(full_cmd);
		// std::cout << "-" << cmd << std::endl;
		while (cmd.find("PASS ") > 0)
			cmd.erase(cmd.begin());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		// std::cout << "-" << cmd << std::endl;
		cmd.erase(cmd.begin(), cmd.begin() + 5);
		// std::cout << "-" << cmd << std::endl;
		//ALREADY_REGISTER
		if (it->pareturn_string_ok == 1){
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
			it->pareturn_string_ok = true;
	}
	if (full_cmd.find("NICK ") < std::string::npos){
		std::string cmd(full_cmd);
		while (cmd.find("NICK ") > 0)
			cmd.erase(cmd.begin());
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
		std::string cmd = full_cmd;
		while (cmd.find("USER ") > 0)
			cmd.erase(cmd.begin());
		if (cmd.find("\r\n") < std::string::npos)
			cmd.erase(cmd.begin() + cmd.find("\r\n"), cmd.end()); 
		else
			cmd.erase(cmd.begin() + cmd.find("\n"), cmd.end()); 
		cmd.erase(cmd.begin(), cmd.begin() + 5);	
		char **cmds = ft_split(cmd.c_str(), ' ');
		int i = -1; while (cmds[++i]) ;
		//NEED_MORE_PARAM
		if (i < 4) {
			std::string rep = error_needmoreparams("USER ");
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
	// std::cout << "\n" << full_cmd << "ID CLIENT :" << it->id  << "\nPASS :" << it->pass_ok << "\nNICK = " << it->nick << "\nUSER :" << it->name << "\nREAL_NAME :" << it->realname << "\n"<< std::endl;
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
				it->is_operator = true;
				std::string rep = rplyouroper(it->nick);
				send(id, rep.c_str(), rep.size(), 0);
				return ;
			}
			else {
				std::string rep = error_pass(it->nick);
				send(id, rep.c_str(), rep.size(), 0);
				return ;
			}
		}
	}
};

void			User::execPING(std::string cmd, unsigned int id){
	(void)cmd;
	std::string rep = rplping();
	send(id, rep.c_str(), rep.size(), 0);
}

void			User::execJOIN(std::string cmd, unsigned int id){
	FINDUSER
	std::cout << cmd.c_str() << std::endl;
	NBARGUMENT(cmd.c_str())
	std::vector<std::string> chan;
	std::vector<std::string> key;

	//ERROR_NEEDMOREPARAMS
	std::cout << nb_cmd << std::endl;
	if (nb_cmd < 2){
		std::string rep = error_needmoreparams("JOIN");
		send(id, rep.c_str(), rep.size(), 0);
	}
	//PARSING
	else {
	//USER_CHAN_PARSING
		std::string dup(cmd);
		for (int i = 0; i < 5; i++)
			dup.erase(dup.begin());
		if (dup.find(" ") < std::string::npos)
			dup.erase(dup.begin() + dup.find(" "), dup.end());
		while (dup.find(",") < std::string::npos){
			std::string tmp = "";
			for (int i = 0; i < (int)dup.find(","); i++)
				tmp.insert(tmp.end(), dup[i]);	
			chan.push_back(tmp);
			for (int i = 0; i < (int)dup.find(",");){
				dup.erase(dup.begin());
			}
			dup.erase(dup.begin());
		}
		if (dup.find(" ") < std::string::npos){
			std::string tmp = "";
			for (int i = 0; i < (int)dup.find(" "); i++)
				tmp.insert(tmp.end(), dup[i]);
			chan.push_back(tmp);
		}
		else {
			std::string tmp = "";
			int i = 0;
			for (std::string::iterator iter = dup.begin(); iter != dup.end(); iter++, i++)
				tmp.insert(tmp.end(), dup[i]);
			chan.push_back(tmp);
		}
		//USER_KEY_PARSING
		dup = cmd;
		for (int i = 0; i < 5; i++)
			dup.erase(dup.begin());
		if (dup.find(" ") < std::string::npos)
		{
			for (int i = 0; i < (int)dup.find(" ");)
				dup.erase(dup.begin());
			dup.erase(dup.begin());
			while (dup.find(",") < std::string::npos) {
				std::string tmp = "";
				for (int i = 0; i < (int)dup.find(","); i++)
					tmp.insert(tmp.end(), dup[i]);	
				key.push_back(tmp);
				for (int i = 0; i < (int)dup.find(",");)
					dup.erase(dup.begin());
				dup.erase(dup.begin());
			}	
			if (dup.find(" ") < std::string::npos){
				std::string tmp = "";
				for (int i = 0; i < (int)dup.find(" "); i++)
					tmp.insert(tmp.end(), dup[i]);
				key.push_back(tmp);
			}
			else if (dup.find("\r") < std::string::npos){
				std::string tmp = "";
				for (int i = 0; i < (int)dup.find("\r"); i++)
					tmp.insert(tmp.end(), dup[i]);
				key.push_back(tmp);
			}
			else {
				std::string tmpo = "";
				int i = 0;
				for (std::string::iterator iter = dup.begin(); iter != dup.end(); iter++, i++)
					tmpo.insert(tmpo.end(), dup[i]);
				key.push_back(tmpo);
			}
		}
	}
	//JOIN_CHANNEL_OR_CREATE
	for (int i = 0; i < (int)chan.size(); i++){
		int j = 0;
		while (j < NUMBER_CHANNEL_MAX){
			if (_channel[j].getName() == chan[i])
				if (_channel[j])
				
		}
	}

	//AFFICHAGE_PARSING
	for(int i = 0; i < (int)chan.size(); i++){
		std::cout << "--" << chan[i] << std::endl;
	}
	for(int i = 0; i < (int)key.size(); i++)
		std::cout << "==" << key[i] << std::endl;
}