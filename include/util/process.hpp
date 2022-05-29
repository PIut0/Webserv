#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Fileio.hpp"

void Process(FdInterface *target, struct kevent event);

#endif
