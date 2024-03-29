#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

#include"../includes/ft_irc.hpp"

typedef struct s_uer t_user;

class Channel {
	private :
		std::string					_name;
		std::string					_key;
		std::vector<unsigned int>	_current_user;
		std::vector<unsigned int>	_invite_list;
		std::vector<unsigned int>	_operator;
		std::vector<unsigned int>	_mutted_user;
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
		void			inviteOnlySetTrue() {_invite_only = true; this->inviteOnlyInit();};
		void			inviteOnlySetFalse() {_invite_only = false; _invite_list.clear();};
		void			setKey(std::string key) {_key = key;};
		void			setName(std::string name) {_name = name;};
		void			setTopic(std::string topic) {_topic = topic; _topic_set = true;};
		//MEMBRE_FUCT
		int				join(unsigned int id, std::string nick, std::string key, std::vector<t_user>::iterator the, std::vector<t_user>& Users);
		void			checkMU();
		int				initChannel(unsigned int id, std::string channel_name, std::string channel_key);
		int				userLeave(unsigned int id, std::string rep);
		int				userIsInChannel(unsigned int id);
		int				userIsOperator(unsigned int id);
		int				userDisconnect(unsigned int id, std::string rep);
		int				userKick(unsigned int id, unsigned int trig, std::string user, std::string message, std::string rep);
		int				sendMessage(std::string message,std::string nick,std::string name, unsigned int id_s);
		int				setOperator(unsigned int id, std::string channel, std::string user);
		int				unsetOperator(unsigned int id, std::string channel, std::string user);
		void			printOperator(void);
		void			notifIsOperator(std::string channel, std::string user);
		void			notifIsNoLongerOperator(std::string channel, std::string user);
		int				userIsBan(std::string nick);
		int				banUser(std::string channel, std::string nick, unsigned int id_banned, std::string rep);
		int				unBanUser(std::string channel, std::string nick, unsigned int id_banned);
		int				kickUser(std::string channel, std::string nick, unsigned int id_banned, std::string rep);
		int				muteUser(unsigned int id_mutted);
		int				unMuteUser(unsigned int id_mutted);
		int				userIsMute(unsigned int id_mutted);
		int				sendToAllUser(std::string message);
		int				inviteOnlyInit();
		int				inviteUser(unsigned int id);

};
#endif
