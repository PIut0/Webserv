#include <iostream>
#include <string>

typedef int HttpCommandType;

#define HEADER_TYPE_RANGE 2

#define GENERAL   0
#define ENTITY    1
#define RESPONSE  2
#define REQUEST   3


HttpCommandType GetCommandType(std::string &command)
{
  return REQUEST << HEADER_TYPE_RANGE | REQUEST;
}

int main()
{
  std::string hi = "hi";
  std::cout << "num : " << GetCommandType(hi) << std::endl;
}

