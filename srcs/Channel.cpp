#include"../includes/Channel.hpp"

Channel::Channel(): _name("Default"), _key("Default_key"), _in_use(false), _invite_only(false), _use_key(false){
};

Channel::Channel(std::string name, std::string key) : _name(name), _key(key), _in_use(false), _invite_only(false), _use_key(false){
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


void			Channel::getName() const {
	std::cout << this->_name << std::endl;
};