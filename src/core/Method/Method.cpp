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
  close(target_fd);
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

int Method::IsDir(const std::string &path)
{
  return (path.back() == '/');
}

void Method::SetResponseStatus(ResponseHeader *response, const int code)
{
  if (!response)
    return ;
  response->SetItem("Status", StatusCode(code));
}

void Method::ResponseErrorPage()
{
  if ((interface_fd = open(location->error_page.c_str(), O_RDONLY)) < 0) {
    data = DefaultErrorPage(ft_stoi(response->status_code));
    SetResponseMessage();
    kq.AddEvent(target_fd, EVFILT_WRITE, this);
  }
  else {
    if (!IsEOF(interface_fd)) {
      SetResponseMessage();
      kq.AddEvent(target_fd, EVFILT_WRITE, this);
    }
    else {
      fcntl(interface_fd, F_SETFL, O_NONBLOCK);
      kq.AddEvent(interface_fd, EVFILT_READ, this);
    }
  }
}

void Method::SetResponseMessage()
{
  response->SetBody(data);

  if (response->status_code == "") {
    if (response->body.size() > 0)
      SetResponseStatus(response, 200);
    else
      SetResponseStatus(response, 204);
  }

  if (ft_stoi(response->status_code) >= 400)
    response->SetItem("Content-Type", "text/html");

  if (response->body.size() > 0) {
    response->SetItem("Content-Length", ft_itos(response->body.size()));

    if (response->FindItem("Content-Type") == response->conf.end()) {
      if (request && request->host.size() && request->host.find_last_of(".") != std::string::npos)
        response->SetItem("Content-Type", MimeType(request->host.substr(request->host.find_last_of(".") + 1)));
      else
        response->SetItem("Content-Type", "application/octet-stream");
    }
  }

  if (request && request->FindItem("Connection")->first == "Connection")
    response->SetItem("Connection", request->FindItem("Connection")->second->value);
  else if (response->FindItem("Connection") == response->conf.end())
    response->SetItem("Connection", "keep-alive");
}
