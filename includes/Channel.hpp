#ifndef CHANNEL_HPP_
# define CHANNEL_HPP_

#include"../includes/ft_irc.hpp"

class Channel {
	private :
		std::string					_name;
		std::string					_key;
		std::vector<unsigned int>	_current_user;
		std::vector<std::string>	_banned_user;
		bool						_in_use;
		bool						_invite_only;
		bool						_use_key;

	public :
		//CONSTRUCTOR
		Channel();
		Channel(std::string name, std::string key);
		~Channel();
		Channel(const Channel &copy);
		Channel			&operator=(const Channel &copy);
		//MEM_nameBER_FUCT
		void			getName() const;
};
#endif
