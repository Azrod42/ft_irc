#include"../includes/ft_irc.hpp"

#define IPADDR "127.0.0.1"
#define ERR_UNKNOWCOMMAND " :Unknow command\n"

//ERR_PAreturn_stringWDMISMATCH
std::string error_pass(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 464 " << nick << " :Password incorrect" << "\r\n";
	return (return_string.str());
}

// #define ERR_ALREADYREGISTRED " :Unauthorized command (already registered)"
std::string error_alreadyregistred(void){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 462 " << " :Unauthorized command (already registered)" << "\r\n";
	return (return_string.str());
}

// #define ERR_NEEDMOREPARAMS " :Not enough parameters\n" // need <command> avant l'erreur
std::string error_needmoreparams(std::string word){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 461 " << word << " :Not enough parameters" << "\r\n";
	return (return_string.str());
}

// #define ERR_ERRONEUSNICKNAME 432
std::string error_erroneusnickname(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 432 " << nick << " :Erroneous nickname" << "\r\n";
	return (return_string.str());
}

// #define ERR_UNKNOWNCOMMAND 421
std::string error_unknowcommand(std::string cmd){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 421 " << cmd << " :Unknown command" << "\r\n";
	return (return_string.str());
}

// #define ERR_NONICKNAMEGIVEN 431
std::string error_nonicknamegiven(void){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 431 " << ":No nickname given" << "\r\n";
	return (return_string.str());
}

// #define ERR_NICKNAMEINUSE 433
std::string error_nicknameinuse(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 433 " << nick << " :Nickname is already in use" << "\r\n";
	return (return_string.str());
}

// #define RPL_WELCOME 001
std::string rplwelcome(std::string nick, std::string user){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 001 " << nick << " Welcome to the Internet Relay Network " << nick << "!" << user << "@" << IPADDR << "\r\n";
	return (return_string.str());
}

// #define RPL_YOUREOPER 381
std::string rplyouroper(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 381 " << nick << " :You are now an IRC operator" << "\r\n";
	return (return_string.str());
}

// #define ERR_NOOPERHOST 491
std::string error_nooperhost(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 491 " << nick << " :No O-lines for your host" << "\r\n";
	return (return_string.str());
}

// #define ERR_NOSUCHSERVER 402
std::string error_nosuchserver(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 402 " << nick << " :No such server" << "\r\n";
	return (return_string.str());
}

// PING REPLY
std::string rplping(void){
	std::stringstream return_string;

	return_string << "PONG " << IPADDR << "\r\n";
	return (return_string.str());
}

// JOIN REPLY
std::string rpljoin(std::string nick, std::string name, std::string channel){
	std::stringstream return_string;

	return_string << ":" << nick <<  "!" << name << "@" << IPADDR << " JOIN " << channel << "\r\n";
	return (return_string.str());
}

//#define ERR_TOOMANYCHANNELS 405
std::string error_toomanychannels(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 405 " << channel_name << " :You have joined too many channels" << "\r\n";
	return (return_string.str());
}

//#define RPL_NOTOPIC 331
std::string rplnotopic(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 331 " << "a " << channel_name << "\r\n";
	return (return_string.str());
}

//#define RPL_NOTOPIC 332
std::string rpltopic(std::string channel_name, std::string topic){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 332 " << channel_name << " : " << topic << "\r\n";
	return (return_string.str());
}

//#define ERR_BADCHANNELKEY 475
std::string error_badchannelkey(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 475 " << channel_name << " :Cannot join channel (+k)" << "\r\n";
	return (return_string.str());
}

//#define ERR_INVITEONLYCHAN 473
std::string error_inviteonlychan(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 473 " << channel_name << " :Cannot join channel (+i)" << "\r\n";
	return (return_string.str());
}

//#define ERR_BANNEDFROMCHAN 474
std::string error_bannedformchan(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 473 " << channel_name << " :Cannot join channel (+b)" << "\r\n";
	return (return_string.str());
}

//#define ERR_NOTONCHANNEL 442
std::string error_notonchannel(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 442 " << channel_name << " :You're not on that channel" << "\r\n";
	return (return_string.str());
}

//#define ERR_NOSUCHCHANNEL 403
std::string error_nosuchchannel(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 403 " << channel_name << " :No such channel" << "\r\n";
	return (return_string.str());
}	

std::string rpl(std::string nick, std::string channel, std::string name, std::string message){
	std::stringstream return_string;

	return_string << ":" << nick <<  "!" << name << "@" << IPADDR << " PRIVMSG "<< channel << " " << message << "\r\n";
	return (return_string.str());
}

//#define ERR_CANNOTSENDTOCHAN 404
std::string error_cannotsendtochan(std::string channel_name){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 404 " << channel_name << " :Cannot send to channel" << "\r\n";
	return (return_string.str());
}	

//#define ERR_NORECIPIENT 411
std::string error_norecipient(std::string commande){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 411 " << ":No recipient given (" << commande << ")" << "\r\n";
	return (return_string.str());
}	

//#define ERR_NOTEXTTOSEND 412
std::string error_notexttosend(void){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 412 " << ":No text to send" << "\r\n";
	return (return_string.str());
}	

//#define ERR_NOSUCHNICK 401
std::string error_nosuchnick(std::string nick){
	std::stringstream return_string;

	return_string << ":" << IPADDR << " 401 " << nick << " :No such nick/channel" << "\r\n";
	return (return_string.str());
}	