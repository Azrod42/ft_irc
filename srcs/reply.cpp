#include"../includes/ft_irc.hpp"

#define IPADDR "127.0.0.1"
#define ERR_UNKNOWCOMMAND " :Unknow command\n"

//ERR_PAreturn_stringWDMISMATCH
std::string error_pareturn_string(std::string nick){
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

