#include "Header.hpp"

Header::Header()
{
  this->general_ = new HTTPGeneralHeader();
  this->entitiy_ = new HTTPEntityHeader();
}

Header::~Header() {}

void Header::SetUpHeaderGeneral(HttpContentsType &type)
{
  switch (H_MASKING(type))
  {
    case H_DATE:
      break;
    case H_PRAGMA:
      break;
    case H_CACHE_CONTROL:
      break;
    case H_VIA:
      break;
    case H_CONNECTION:
      break;
    case H_TRANSFER_ENCODING:
      break;
    default: // If the masking range is wrong, you can handle the error here.
      std::cerr << "error" << std::endl;
      break;
  }
}

// TODO case 채워넣기
void Header::SetUpHeaderEntity(HttpContentsType &type)
{
  switch (H_MASKING(type))
  {
    case H_DATE:
      break;
    case H_PRAGMA:
      break;
    case H_CACHE_CONTROL:
      break;
    case H_VIA:
      break;
    case H_CONNECTION:
      break;
    case H_TRANSFER_ENCODING:
      break;
    default: // If the masking range is wrong, you can handle the error here.
      std::cerr << "error" << std::endl;
      break;
  }
}
