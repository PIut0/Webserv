#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include <string>
#include <vector>
#include "System.hpp"
#include "utils.hpp"

#include <iostream> // DEBUG

class LocationBlock : public System
{
public:
	LocationBlock();
	LocationBlock(std::string&, std::vector<std::string>&);
	~LocationBlock();

	enum LocationAttribute {
		ERROR = -1,
		ALLOW_METHOD,
		INDEX,
		AUTO_INDEX,
		CGI_INFO,

	};

private:

	std::string location_path;
	// std::string root;		  // root directory path
	// std::string allow_method; //
	// std::string index;		  //
	// std::string autoindex;	  // boolean?
	// std::string cgi;		  // .extension

	void init_location_block(const std::string&, std::vector<std::string>&);
	LocationAttribute check_validate(const std::string&);
};


#endif
