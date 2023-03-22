#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

#include"../includes/ft_irc.hpp"

class Channel {
	private :
		std::string					_name;
		std::string					_key;
		std::vector<unsigned int>	_current_user;
		std::vector<unsigned int>	_invite_list;
		std::vector<std::string>	_banned_user;
		bool						_in_use;
		bool						_invite_only;
		bool						_use_key;
		std::string					_topic;
		bool						_topic_set;

	public :
		//CONSTRUCTOR
		Channel();
		Channel(std::string name, std::string key);
		~Channel();
		Channel(const Channel &copy);
		Channel			&operator=(const Channel &copy);
		//ACCES
		std::string		getName() const{return (_name);};
		std::string		getKey() const {return (_key);}
		std::string		getTopic() const {return (_topic);}
		bool			useKey() const {return (_use_key);};
		bool			inviteOnly() const {return (_invite_only);};
		bool			inUse() const {return (_in_use);};
		bool			istopic() const {return (_topic_set);};
		//SETTER
		void			useKeySetTrue() {_use_key = true;};
		void			useKeySetFalse() {_use_key = false;};
		void			inviteOnlySetTrue() {_invite_only = true;};
		void			inviteOnlySetFalse() {_invite_only = false;};
		void			setKey(std::string key) {_key = key;};
		void			setName(std::string name) {_name = name;};
		//MEMBRE_FUCT
		int				join(unsigned int id, std::string nick, std::string key);
		int				initChannel(unsigned int id, std::string channel_name, std::string channel_key);
		int				userLeave(unsigned int id);
		int				userDisconnect(unsigned int id);
		int				sendMessage(std::string message,std::string nick,std::string name, unsigned int id_s);
};
#endif
