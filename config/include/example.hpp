#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <iostream>
#include <string>
#include <exception>

const std::string location_valid_config[] = {"root", "allow_method", "index", "autoindex", "cgi"};

class UnvalidConfigException : public std::exception
{
private:
	std::string error_msg;

public:
	UnvalidConfigException(const std::string &error_msg, const std::string &additional_msg) : error_msg(error_msg + " : " + additional_msg) {}

	virtual const char *what() const throw()
	{
		return error_msg.c_str();
	}
}

class Location
{
private:
	std::string root;		  // root directory path
	std::string allow_method; //
	std::string index;		  //
	std::string autoindex;	  // boolean?
	std::string cgi;		  // .extension

public:
	Location();
	~Location();
};

class Server
{
private:
	Location location[];	 //
	std::string server_name; //
	unsigned int port;		 // uintshort?
	std::string host;		 //

	unsigned int limit_client_body_size; //
	unsigned int req_uri_limit_size;	 //
	unsigned int req_header_limit_size;	 //
	std::string default_err_page;

public:
	Server();
	~Server();
};

class ServerManager
{
private:
	Server server[];

public:
	ServerManager();
	~ServerManager();
};

#endif