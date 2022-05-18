#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include <string>
#include <vector>
#include "System.hpp"

#include <iostream> // DEBUG

class LocationBlock : public System
{
private:
	std::string location_path;
	// std::string root;		  // root directory path
	// std::string allow_method; //
	// std::string index;		  //
	// std::string autoindex;	  // boolean?
	// std::string cgi;		  // .extension

	void init_location_block(std::string&, std::vector<std::string>&);

public:
	LocationBlock();
	LocationBlock(std::string&, std::vector<std::string>&);
	~LocationBlock();
};


#endif
