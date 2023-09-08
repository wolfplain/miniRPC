#ifndef MINIRPC_RPC_SERVICE_H
#define MINIRPC_RPC_SERVICE_H
#include "common/errorno.h"

struct Request {
    std::string serviceName;
    Document doc;
};

struct Response {
    std::string message;
};


class RpcService {
public:
    
    virtual ErrorNo Process(const Request &req, Response &rsp) = 0;
    bool CheckService(const Request &req);
    std::string GetServiceName() const
    {
        return serviceName_;
    }

    RpcService(std::string serviceName) : serviceName_(serviceName) {}
private:
    std::string serviceName_;
};
#endif  // MINIRPC_RPC_SERVICE_H

