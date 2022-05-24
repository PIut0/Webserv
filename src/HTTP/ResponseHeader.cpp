#include "ResponseHeader.hpp"

ResponseHeader::ResponseHeader() : Header()
{
  this->response_ = new HTTPResponseHeader();
}

ResponseHeader::~ResponseHeader() {}

void ResponseHeader::SetUpHeaderResponse(HttpContentsType &type)
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

void ResponseHeader::SetUpHeader(HttpContentsType &type)
{
  switch (G_MASKING(type)) {
    case G_GENERAL:
      SetUpHeaderGeneral(type);
      break;
    case G_ENTITY:
      SetUpHeaderEntity(type);
      break;
    case G_RESPONSE:
      SetUpHeaderResponse(type);
      break;
    default: // If the masking range is wrong, you can handle the error here.
      std::cerr << "error" << std::endl;
      break;
  }
}

