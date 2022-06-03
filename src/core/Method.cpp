#include "Method.hpp"
#include "utils.hpp"

Method::Method(KQueue &kq, Client *client, FdInterfaceType type) : FdInterface(kq, type), data(""), client(client)
{
  target_fd = client->interface_fd;
  request = (client->request) ? client->request : new RequestHeader();
  response = (client->response) ? client->response : new ResponseHeader();
  location = client->GetLocationBlock();
}

Method::~Method()
{
  delete request;
  delete response;
  close(interface_fd);
}

int Method::EventRead()
{
  char buf[1024];
  int n = read(interface_fd, buf, sizeof(buf) - 1);
  buf[n] = '\0';
  data += buf;

  return IsEOF(interface_fd);
}

int Method::EventWrite()
{
  std::string res = response->ToString();
  int n = write(target_fd, res.c_str(), res.size());
  if (n <= 0)
    return n;
  res = res.substr(n);
  n = res.size();

  return n;
}

void Method::SetResponseMessage()
{
  if (response->status_code == "")
    response->SetItem("Status", StatusCode(200));

  response->SetBody(data);
  response->SetItem("Content-Length", ft_itos(response->body.size()));

  if (response->body.size() > 0) {
    if (response->FindItem("Content-Type") == response->conf.end()) {
      if (request && request->host.size() && request->host.find_last_of(".") != std::string::npos)
        response->SetItem("Content-Type", MimeType(request->host.substr(request->host.find_last_of(".") + 1)));
      else
        response->SetItem("Content-Type", "application/octet-stream");
    }
  }

  if (request && request->FindItem("Connection")->first == "Connection")
    response->SetItem("Connection", request->FindItem("Connection")->second->value);
  else
    response->SetItem("Connection", "keep-alive");
}
