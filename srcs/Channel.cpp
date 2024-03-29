#include"../includes/Channel.hpp"

Channel::Channel()
: _name("Default"), _key("Default_key"), _in_use(false), _invite_only(false), _use_key(false), _topic(""), _topic_set(false){
	_current_user.push_back(99999);
	_banned_user.push_back("__DEFAULT__");
	_invite_list.push_back(99999);
};

Channel::Channel(std::string name, std::string key)
: _name(name), _key(key), _in_use(false), _invite_only(false), _use_key(false), _topic(""), _topic_set(false){
};

Channel::~Channel(){
};

Channel::Channel(const Channel &copy){
	*this = copy;
};

Channel		   &Channel::operator=(const Channel &copy) {
	this->_name = copy._name;
	this->_key = copy._key;
	this->_current_user = copy._current_user;
	this->_in_use = copy._in_use;
	this->_invite_only = copy._invite_only;
	this->_use_key = copy._use_key;
	return (*this);
};

//RET 0 = USER_JOIN_CHANNEL
//RET 1 = WRONG_KEY
//RET 2 = INVITE_LIST_USER_NOT_IN
//RET 3 = USER_BANNED_FORM_CHANNEL
//RET 4 = USER_ALREADY_IN_CHANNEL
int				Channel::join(unsigned int id, std::string nick, std::string key, std::vector<t_user>::iterator the, std::vector<t_user>& Users){
	// //std::cout<< "channel join " << std::endl;
	if (_use_key == true)
	{
		// //std::cout << "-=-=-" << key << " " << _key << std::endl;
		if (key != _key)
			return (1);
	}
	if (_invite_only == true){
		std::vector<unsigned int>::iterator it = _invite_list.begin();
		for (; it != _invite_list.end(); it++) { if (*it == id) break;};
		if (it == _invite_list.end())
			return (2);
	}
	std::vector<std::string>::iterator it3 = _banned_user.begin();
	for (; it3 != _banned_user.end(); it3++) {
		// //std::cout << *it3 << " - " << nick << std::endl;
		if (*it3 == nick)
			return (3);
		}
	
	std::vector<unsigned int>::iterator it2 = _current_user.begin();
	for (; it2!= _current_user.end(); it2++) { if (*it2 == id) return (4);}
	_current_user.push_back(id);
	std::string rep = rpljoin(the->nick, the->name, this->_name);
	// -----
	for (size_t i4 = 0; i4 < this->_current_user.size(); i4++)
		send(this->_current_user[i4], rep.c_str(), rep.size(), 0);
	for (size_t i4 = 0; i4 < this->_current_user.size(); i4++)
	{
		if (this->_current_user[i4] != the->id){
			for (size_t i5 = 0; i5 < Users.size(); i5++)
			{
				if (Users[i5].id == this->_current_user[i4]){
					rep = rpljoin(Users[i5].nick, Users[i5].name, this->_name);
					send(the->id, rep.c_str(), rep.size(), 0);
					break ;
				}
			}
		}
	}
	
	// -----
	// //std::cout<< "OK join" << std::endl;
	return (0);
};

int				Channel::initChannel(unsigned int id, std::string channel_name, std::string channel_key){
	_name = channel_name;
	if (channel_key == "__NOKEY__")
		this->useKeySetFalse();
	else {
		this->useKeySetTrue();
		this->setKey(channel_key);
	}
	_current_user.clear();
	_current_user.push_back(id);
	_operator.push_back(id);
	// //std::cout << "Channel created\nNAME : " << channel_name <<  "\nKEY : " << channel_key << std::endl;
	return (0);
};

int				Channel::userLeave(unsigned int id, std::string rep){
	std::vector<unsigned int>::iterator it = _current_user.begin();

	while (it != _current_user.end()){
		if (*it == id){
			break;
		}
		it++;
	}
	if (it == _current_user.end())
		return 1;
	if (_current_user.size() > 1) {
		for (size_t i = 0; i < _current_user.size(); i++)
			send(_current_user[i], rep.c_str(), rep.size(), 0);
	}
	_current_user.erase(it);
	return 0;
};

//ret 1 = ERR_CANNOTSENDTOCHAN (user not in)
int				Channel::sendMessage(std::string message ,std::string nick,std::string name, unsigned int id_s){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	std::vector<unsigned int>::iterator itin = _current_user.begin();
	int user_in = 0;
	
	while (itin != _current_user.end()){
		// //std::cout << "user :" <<  *itin << std::endl;
		itin++;
	}
	// this->checkMU();
	// itin = _current_user.begin();
	// while (itin != _current_user.end()){
	// 	//std::cout << "user :" <<  *itin << std::endl;
	// 	itin++;
	// }
	itin = _current_user.begin();
	while (itin != _current_user.end()){
		if (*itin == id_s)
			user_in = 1;
		itin++;
	}
	// //std::cout << "User in :" << user_in << std::endl;
	if (!user_in)
		return (1);
	while (it != _current_user.end()){
		if (*it != id_s && *it != 99999){
			std::string rep = rpl(nick, this->getName(), name, message);
			// //std::cout << "Message send to :" << *it << "\n" << rep << std::endl;
			send(*it, rep.c_str(), rep.size(), 0);
		}
		it++;
	}
	return (0);
};

int				Channel::userDisconnect(unsigned int id, std::string rep){
	{
		std::vector<unsigned int>::iterator it = _current_user.begin();

		// //std::cout << "UI " << _current_user.size() << " id : " << id << std::endl;
		if (_current_user.size() < 1)
			return (0);
		while (it != _current_user.end()){
			// //std::cout << this->_name << *it << std::endl;
			if (*it == id){
				this->userLeave(id, rep);
				//_current_user.erase(it);
				return (0);
			}
			it++;
		}
	}
	{
		std::vector<unsigned int>::iterator it = _invite_list.begin();

		if (_invite_list.size() < 1)
			return (0);
		while (it != _invite_list.end()){
			if (*it == id){
				_invite_list.erase(it);
				return (0);
			}
			it++;
		}
	}
	// {
	// 	std::vector<unsigned int>::iterator it = _operator.begin();

	// 	if (_operator.size() < 1)
	// 		return (0);
	// 	while (it != _operator.end()){
	// 		if (*it == id){
	// 			_operator.erase(it);
	// 			return (0);
	// 		}
	// 		it++;
	// 	}
	// }
	{
		std::vector<unsigned int>::iterator it = _mutted_user.begin();

		if (_mutted_user.size() < 1)
			return (0);
		while (it != _mutted_user.end()){
			if (*it == id){
				_mutted_user.erase(it);
				return (0);
			}
			it++;
		}
	}
	return (0);
};

int				Channel::userKick(unsigned int id, unsigned int trig, std::string user, std::string message, std::string repl){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	std::vector<unsigned int>::iterator it2 = _current_user.begin();
	std::string mess;

	if (message.size() < 1)
		mess = "nan";
	else
		mess = message;
	int trigger_in = 0;
	int kicked_in = 0;
	while (it != _current_user.end()){
		if (*it == id){
			kicked_in = 1;
			break;
		}
		it++;
	}
	while (it2 != _current_user.end()){
		if (*it2 == trig){
			trigger_in = 1;
			break;
		}
		it2++;
	}
	if (trigger_in == 1 && kicked_in == 1){
		std::vector<unsigned int>::iterator it3 = _current_user.begin();
		std::string rep = rplkicked(user, this->getName(), mess);
		while (it3 != _current_user.end()){
			send(*it3, rep.c_str(), rep.size(), 0);
			it3++;
		}
		this->userLeave(*it, repl);
		//_current_user.erase(it);
		return 0;
	} else if (!trigger_in){
		return 2;
	}
	return (1);
};

void			Channel::checkMU(){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	for (;it != _current_user.end(); it++) {
		std::vector<unsigned int>::iterator it2 = _current_user.begin();
		// //std::cout << *it << " at " << &it << "\n" << *it2 << " at " << &it2 << std::endl;
		while (it2 != _current_user.end()) {
			if (*it == *it2 && it != it2){
				_current_user.erase(it2);
				it2--;
			}
			it2++;
		}
	}
};

int				Channel::userIsInChannel(unsigned int id){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	while (it != _current_user.end()) {
		if (*it == id)
			return (0);
		it++;
	}
	return (1);
};

int				Channel::userIsOperator(unsigned int id){
	std::vector<unsigned int>::iterator it = _operator.begin();
	while (it != _operator.end()) {
		if (*it == id)
			return (1);
		it++;
	}
	return (0);
};

int				Channel::setOperator(unsigned int id, std::string channel, std::string user){
	std::vector<unsigned int>::iterator it = _operator.begin();
	while (it != _operator.end()){
		if (*it == id){
			//std::cout << "Already OP" << std::endl;
			return (2);
		}
		it++;
	}
	this->_operator.push_back(id);
	this->notifIsOperator(channel, user);
	return (0);
};

int				Channel::unsetOperator(unsigned int id, std::string channel, std::string user){
	std::vector<unsigned int>::iterator it = _operator.begin();
	while (it != _operator.end()){
		if (*it == id)
			break;
		it++;
	}
	if (it == _operator.end())
		return (2);
	_operator.erase(it);
	this->notifIsNoLongerOperator(channel, user);
	return (0);
};

void			Channel::printOperator(void) {
	std::vector<unsigned int>::iterator it = _operator.begin();
	//std::cout << "---------------\nOperator :" << std::endl;
	while (it != _operator.end()){
		//std::cout << *it << std::endl;
		it++;
	}
	//std::cout << "---------------" <<std::endl;
}

void			Channel::notifIsOperator(std::string channel, std::string user) {
	std::vector<unsigned int>::iterator it = _current_user.begin();
	while (it != _current_user.end()){
		std::string rep = rpluserisnowoper(channel, user);
		send(*it, rep.c_str(), rep.size(), 0);
		it++;
	}
}

void			Channel::notifIsNoLongerOperator(std::string channel, std::string user) {
	std::vector<unsigned int>::iterator it = _current_user.begin();
	while (it != _current_user.end()){
		std::string rep = rpluserisnolongeroper(channel, user);
		send(*it, rep.c_str(), rep.size(), 0);
		it++;
	}
}

int				Channel::userIsBan(std::string nick){
	std::vector<std::string>::iterator it = _banned_user.begin();
	while (it != _banned_user.end()) {
		if (*it == nick)
			return (1);
		it++;
	}
	return (0);
};

int				Channel::userIsMute(unsigned int id){
	std::vector<unsigned int>::iterator it = _mutted_user.begin();
	while (it != _mutted_user.end()) {
		if (*it == id)
			return (1);
		it++;
	}
	return (0);
};

int				Channel::banUser(std::string channel, std::string nick, unsigned int id_banned, std::string rep){
	_banned_user.push_back(nick);
	std::vector<unsigned int>::iterator it = _current_user.begin();	
	while (it != _current_user.end()){
		std::string rep = rplusergetban(channel, nick);
		send(*it, rep.c_str(), rep.size(), 0);
		it++;
	}
	it = _current_user.begin();
	while (it != _current_user.end()){
		if (*it == id_banned)
			break;
		it++;
	}
	if (it != _current_user.end())
		this->userLeave(id_banned, rep);
		//_current_user.erase(it);
	return (0);
};

int				Channel::unBanUser(std::string channel, std::string nick, unsigned int id_banned){
	std::vector<unsigned int>::iterator it = _current_user.begin();	
	(void)id_banned;
	while (it != _current_user.end()){
		std::string rep = rplusergetunban(channel, nick);
		send(*it, rep.c_str(), rep.size(), 0);
		it++;
	}
	std::vector<std::string>::iterator it2 = _banned_user.begin();
	while (it2 != _banned_user.end()){
		if (*it2 == nick)
			break;
		it2++;
	}
	if (it2 != _banned_user.end())
		_banned_user.erase(it2);
	return (0);
};

int				Channel::kickUser(std::string channel, std::string nick, unsigned int id_banned, std::string rep){
	std::vector<unsigned int>::iterator it = _current_user.begin();	

	while (it != _current_user.end()){
		std::string rep = rplusergetkick(channel, nick);
		send(*it, rep.c_str(), rep.size(), 0);
		it++;
	}
	it = _current_user.begin();
	while (it != _current_user.end()){
		if (*it == id_banned)
			break;
		it++;
	}
	if (it != _current_user.end())
		this->userLeave(id_banned, rep);
		//_current_user.erase(it);
	return (0);
};

int				Channel::muteUser(unsigned int id_mutted){
	std::vector<unsigned int>::iterator it = _mutted_user.begin();

	while (it != _mutted_user.end()){
		if (*it == id_mutted)
			break;
		it++;
	}
	if (it == _mutted_user.end())
		_mutted_user.push_back(id_mutted);
	return (0);
}

int				Channel::unMuteUser(unsigned int id_mutted){
	std::vector<unsigned int>::iterator it = _mutted_user.begin();

	while (it != _mutted_user.end()){
		if (*it == id_mutted)
			break;
		it++;
	}
	if (it != _mutted_user.end())
		_mutted_user.erase(it);
	return (0);
}

int				Channel::sendToAllUser(std::string message){
	std::vector<unsigned int>::iterator it = _current_user.begin();

	while (it != _current_user.end()){
		send(*it, message.c_str(), message.size(), 0);
		it++;
	}
	return (0);
}

int				Channel::inviteOnlyInit(){
	std::vector<unsigned int>::iterator it = _current_user.begin();

	while (it != _current_user.end()){
		_invite_list.push_back(*it);
		it++;
	}
	return (0);
}

int				Channel::inviteUser(unsigned int id){
	std::vector<unsigned int>::iterator it = _invite_list.begin();

	while (it != _invite_list.end()){
		if (*it == id)
			break;
		it++;
	}
	if (it == _invite_list.end()){
		_invite_list.push_back(id);
		return (1); //FOR SEND INVITE TO USER
	}
	return (0);
}