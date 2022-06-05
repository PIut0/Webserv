#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "GetMethod.hpp"
#include "Cgi.hpp"

void Process(FdInterface *target, struct kevent event);

#endif
