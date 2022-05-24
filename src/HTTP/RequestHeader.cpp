#include "RequestHeader.hpp"

RequestHeader::RequestHeader() : Header()
{
  this->request_ = new HTTPRequestHeader();
}

RequestHeader::~RequestHeader() {}

void RequestHeader::SetUpHeaderRequest(HttpContentsType &type)
{
  switch (H_MASKING(type))
  {
    case H_DATE:
      break;
    default: // If the masking range is wrong, you can handle the error here.
      std::cerr << "error" << std::endl;
      break;
  }
}

void RequestHeader::SetUpHeader(HttpContentsType &type)
{
  switch (G_MASKING(type)) {
    case G_GENERAL:
      SetUpHeaderGeneral(type);
      break;
    case G_ENTITY:
      SetUpHeaderEntity(type);
      break;
    case G_REQUEST:
      SetUpHeaderRequest(type);
      break;
    default: // If the masking range is wrong, you can handle the error here.
      std::cerr << "error" << std::endl;
      break;
  }
}

