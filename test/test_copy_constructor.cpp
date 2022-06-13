#include "ServerManager.hpp"
#include "RequestHeader.hpp"
#include "ResponseHeader.hpp"
#include "ServerBlock.hpp"

int main(int argc, char **argv)
{
    { // RequestHeader Copy Constructor Test
        const std::string dummy = "GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1\r\nHost: code.tutsplus.com\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nCookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n";
        RequestHeader *rq1 = new RequestHeader();
        rq1->Parse(dummy);
        RequestHeader rq2(*rq1);
        if (rq1->method != rq2.method ||
            rq1->host != rq2.host ||
            rq1->http_major != rq2.http_major ||
            rq1->http_minor != rq2.http_minor ||
            rq1->body != rq2.body) {
                std::cout << "Fail" << std::endl;
            }
        rq2.GetItem("asdfsadfas");
        for (req_header_it_t it  = rq1->conf.begin() ; it != rq1->conf.end() ; ++it) {
            if (it->first == "_")
                std::cout << "isEmpty" << std::endl;
            if (it->second->value != rq2.conf[it->first]->value) {
                std::cout << "Fail" << std::endl;
            }
        }
        delete rq1;
        rq2.SetItem("User-Agent", "asdf");
        std::cout << "val : [" << rq2.GetItem("User-Agent").value << "]" <<std::endl;
        std::cout << "Done" << std::endl;
    }
    { // ResponseHeader Copy Constructor Test
        const std::string dummy = "Status: 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\nHELLO";
        ResponseHeader rs1;
        rs1.Parse(dummy);
        ResponseHeader rs2(rs1);
        if (rs1.status_code != rs2.status_code ||
            rs1.status_msg != rs2.status_msg ||
            rs1.body != rs2.body) {
                std::cout << "Fail" << std::endl;
            }
        for (size_t i = 0 ; i < rs1.conf.size() ; ++i) {
            if (rs1.conf[i]->value != rs2.conf[i]->value) {
                std::cout << "Fail" << std::endl;
            }
        }
        std::cout << "Done" << std::endl;
    }
    { // ServerBlock
        ServerManager sm1(CheckArg(argc, argv));
        ServerManager sm2(sm1);
        if (sm1.serverBlock.size() != sm2.serverBlock.size()) {
            std::cout << "Fail" << std::endl;
        }
        std::cout << "Done" << std::endl;

        ServerBlock sb(sm1.serverBlock[0]);

        if (sb.server_name != sm1.serverBlock[0].server_name ||
            sb.host != sm1.serverBlock[0].host ||
            sb.port != sm1.serverBlock[0].port) {
                std::cout << "Fail" << std::endl;
            }

        std::cout << "Done" << std::endl;

        LocationBlock lb(sm1.serverBlock[0].location[0]);

        if (lb.location_path != sm1.serverBlock[0].location[0].location_path ||
            lb.root != sm1.serverBlock[0].location[0].root ||
            lb.allow_methods != sm1.serverBlock[0].location[0].allow_methods ||
            lb.auto_index != sm1.serverBlock[0].location[0].auto_index ||
            lb.cgi_info != sm1.serverBlock[0].location[0].cgi_info ||
            lb.error_page != sm1.serverBlock[0].location[0].error_page ||
            lb.request_max_body_size != sm1.serverBlock[0].location[0].request_max_body_size ||
            lb.ret != sm1.serverBlock[0].location[0].ret) {
                std::cout << "Fail" << std::endl;
            }
        for (size_t i = 0 ; i < lb.index.size() ; ++i) {
            if (lb.index[i] != sm1.serverBlock[0].location[0].index[i]) {
                std::cout << "Fail" << std::endl;
            }
        }
        std::cout << "Done" << std::endl;
    }
}
