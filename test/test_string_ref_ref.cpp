#include <string>
#include <iostream>
int main()
{
  std::string tt = "hi";
  std::string &test = tt;
  std::string pp = test;
  std::string &test2 = pp;

  tt = "hihi";
  std::cout << test << std::endl;
  std::cout << test2 << std::endl;

}
