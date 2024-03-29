#ifndef MINIRPC_SERVER_RPCSERVER_H
#define MINIRPC_SERVER_RPCSERVER_H
#include "common/errorno.h"
#include "server/server.h"
#include <vector>

class RpcServer : public WebSocketServer {
public:
    virtual void OnStart(int connectFd);
    virtual void OnMessage(int connectFd, const std::string &message);
    virtual void OnClosed(int connectFd);
    virtual void OnError(int connectFd, ErrorNo errorNo);

    RpcServer(int port) : WebSocketServer(port) {
    }
    ~RpcServer() {
        std::vector<std::string>{}.swap(clientName_);
    }

private:
    std::vector<std::string> clientName_;
};
#endif // MINIRPC_SERVER_RPCSERVER_H