#include "HTTPConfig.hpp"

HTTPConfig::HTTPConfig()
{
  this->data["Date"]                    = SET_CONTENTS(G_GENERAL, H_DATE);
  this->data["Pragma"]                  = SET_CONTENTS(G_GENERAL, H_PRAGMA);
  this->data["Cache-Control"]           = SET_CONTENTS(G_GENERAL, H_CACHE_CONTROL);
  this->data["Via"]                     = SET_CONTENTS(G_GENERAL, H_VIA);
  this->data["Connection"]              = SET_CONTENTS(G_GENERAL, H_CONNECTION);
  this->data["Transfer-Encoding"]       = SET_CONTENTS(G_GENERAL, H_TRANSFER_ENCODING);

  this->data["Content-Type"]            = SET_CONTENTS(G_ENTITY, H_CONTENT_TYPE);
  this->data["Content-Length"]          = SET_CONTENTS(G_ENTITY, H_CONTEHT_LENGTH);
  this->data["Content-Language"]        = SET_CONTENTS(G_ENTITY, H_CONTENT_LANGUAGE);
  this->data["Content-Location"]        = SET_CONTENTS(G_ENTITY, H_CONTENT_LOCATION);
  this->data["Content-Disposition"]     = SET_CONTENTS(G_ENTITY, H_CONTENT_DISPOSITION);
  this->data["Content-Security-Policy"] = SET_CONTENTS(G_ENTITY, H_CONTENT_SECURITY_POLICY);
  this->data["Content-Encoding"]        = SET_CONTENTS(G_ENTITY, H_CONTENT_ENCODING);
  this->data["Location"]                = SET_CONTENTS(G_ENTITY, H_LOCATION);
  this->data["Last-Modified"]           = SET_CONTENTS(G_ENTITY, H_LAST_MODIFIED);
  this->data["Allow"]                   = SET_CONTENTS(G_ENTITY, H_ALLOW);
  this->data["Expires"]                 = SET_CONTENTS(G_ENTITY, H_EXPIRES);
  this->data["ETag"]                    = SET_CONTENTS(G_ENTITY, H_ETAG);

  this->data["Host"]                    = SET_CONTENTS(G_REQUEST, H_HOST);
  this->data["User-Agent"]              = SET_CONTENTS(G_REQUEST, H_USER_AGENT);
  this->data["Referer"]                 = SET_CONTENTS(G_REQUEST, H_REFERER);
  this->data["Accept"]                  = SET_CONTENTS(G_REQUEST, H_ACCEPT);
  this->data["Accept-Charset"]          = SET_CONTENTS(G_REQUEST, H_ACCEPT_CHARSET);
  this->data["Accept-Language"]         = SET_CONTENTS(G_REQUEST, H_ACCEPT_LANGUAGE);
  this->data["Accept-Encoding"]         = SET_CONTENTS(G_REQUEST, H_ACCEPT_ENCODING);
  this->data["If-Modified-Since"]       = SET_CONTENTS(G_REQUEST, H_IF_MODIFIED_SINCE);
  this->data["Authorization"]           = SET_CONTENTS(G_REQUEST, H_AUTHORIZATION);
  this->data["Origin"]                  = SET_CONTENTS(G_REQUEST, H_ORIGIN);
  this->data["Cookie"]                  = SET_CONTENTS(G_REQUEST, H_COOKIE);

  this->data["Server"]                  = SET_CONTENTS(G_RESPONSE, H_SERVER);
  this->data["Age"]                     = SET_CONTENTS(G_RESPONSE, H_AGE);
  this->data["Referrer-Policy"]         = SET_CONTENTS(G_RESPONSE, H_REFERRER_POLICY);
  this->data["WWW-Authenticate"]        = SET_CONTENTS(G_RESPONSE, H_WWW_AUTHENTICATE);
  this->data["Proxy-Authenticate"]      = SET_CONTENTS(G_RESPONSE, H_PROXY_AUTHENTICATE);
  this->data["Set-Cookie"]              = SET_CONTENTS(G_RESPONSE, H_SET_COOKIE);
}

HttpContentsType HTTPConfig::FindType(std::string &command)
{
  return this->data.find(command)->second;
}
