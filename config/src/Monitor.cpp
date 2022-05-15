#include "Monitor.hpp"
#include "utils.hpp"

Monitor::Monitor()
{
    this->logfile.open("log.txt");
}

Monitor::~Monitor()
{
	this->logfile.close();
}

// TODO 시간 출력 형식 바꾸기
time_t Monitor::getTimeStamp() {
    struct timeval time_now;
    gettimeofday(&time_now, nullptr);

    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    return msecs_time;
}

// TODO log 출력 형식 맞추기 [hhmmssms] Scope Content (errno - errstr)
void Monitor::log(const std::string &data)
{
	std::string time = std::to_string(this->getTimeStamp());
	std::string _log = time + " " + "Scope" + " " + data;
	if (errno > 0) {
		_log += "(" + std::to_string(errno) + static_cast<std::string>(std::strerror(errno)) + ")";
	}
	_log += "\n";
	this->logfile.write(_log.c_str(), _log.length());
}
