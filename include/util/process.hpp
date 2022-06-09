#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "GetMethod.hpp"
#include "PutMethod.hpp"
#include "PostMethod.hpp"
#include "DeleteMethod.hpp"

void Process(FdInterface *target, struct kevent event);

#endif
