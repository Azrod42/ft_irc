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
int				Channel::join(unsigned int id, std::string nick, std::string key){
	if (_use_key == true)
		if (key != _key)
			return (1);
	if (_invite_only == true){
		std::vector<unsigned int>::iterator it = _invite_list.begin();
		for (; it != _invite_list.end();) { if (*it == id) break;};
		if (it == _invite_list.end())
			return (2);
	}
	std::vector<std::string>::iterator it3 = _banned_user.begin();
	for (; it3 != _banned_user.end(); it3++) {
		if (*it3 == nick)
			return (3);
		}
	
	std::vector<unsigned int>::iterator it2 = _current_user.begin();
	for (; it2!= _current_user.end(); it2++) { if (*it2 == id) return (4);}
	_current_user.push_back(id);
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
	_current_user.push_back(id);
	std::cout << "Channel created\nNAME : " << channel_name <<  "\nKEY : " << channel_key << std::endl;
	return (0);
};

int				Channel::userLeave(unsigned int id){
	std::vector<unsigned int>::iterator it = _current_user.begin();

	while (it != _current_user.end()){
		if (*it == id){
			_current_user.erase(it);
			return (0);
		}
		it++;
	}
	return (1);
};

//ret 1 = ERR_CANNOTSENDTOCHAN (user not in)
int				Channel::sendMessage(std::string message ,std::string nick,std::string name, unsigned int id_s){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	std::vector<unsigned int>::iterator itin = _current_user.begin();
	int user_in = 0;
	
	while (itin != _current_user.end()){
		std::cout << "user :" <<  *itin << std::endl;
		itin++;
	}
	itin = _current_user.begin();
	while (itin != _current_user.end()){
		if (*itin == id_s)
			user_in = 1;
		itin++;
	}
	// std::cout << "User in :" << user_in << std::endl;
	if (!user_in)
		return (1);
	while (it != _current_user.end()){
		if (*it != id_s && *it != 99999){
			std::string rep = rpl(nick, this->getName(), name, message);
			// std::cout << "Message send to :" << *it << "\n" << rep << std::endl;
			send(*it, rep.c_str(), rep.size(), 0);
		}
		it++;
	}
	return (0);
};

int				Channel::userDisconnect(unsigned int id){
	std::vector<unsigned int>::iterator it = _current_user.begin();

	while (it != _current_user.end()){
		if (*it == id){
			_current_user.erase(it);
			// return (0);
		}
		it++;
	}
	return (0);
};

int				Channel::userKick(unsigned int id, unsigned int trig, std::string user){
	std::vector<unsigned int>::iterator it = _current_user.begin();
	std::vector<unsigned int>::iterator it2 = _current_user.begin();

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
		std::string rep = rplkicked(user, this->getName());
		while (it3 != _current_user.end()){
			std::cout << "Send to :" << *it3 << std::endl;
			send(*it3, rep.c_str(), rep.size(), 0);
			it3++;
		}
		_current_user.erase(it);
		return 0;
	} else if (!trigger_in){
		return 2;
	}
	return (1);
};