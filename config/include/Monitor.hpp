#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

class Monitor
{
public:
	time_t getTimeStamp();
	void log(const std::string&);
	void print(const std::string&);
	void error();

	~Monitor();
	Monitor();
private:

	std::ofstream logfile;
	int _seek;
};

#endif
