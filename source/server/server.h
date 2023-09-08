#ifndef MINIRPC_SERVER_SERVER_H
#define MINIRPC_SERVER_SERVER_H
#include "common/errorno.h"
#include "event/event.h"
#include "connecter.h"

#include <unordered_map>
namespace Server {
class WebSocketServer {
public:
    ErrorNo Start();

    virtual void OnStart(int connectFd) = 0;
    virtual void OnMessage(int connectFd, const std::string &message) = 0;
    virtual void OnClosed(int connectFd) = 0;
    virtual void OnError(int connectFd, ErrorNo errorNo) = 0;

    WebSocketServer(int port = 8080)
        : socketFd_(0), eventFd_(0), serverPort_(port) {}
    ~WebSocketServer();
    std::unordered_map<int, Connecter> GetConnecter();
private:
    ErrorNo ProcessConnect();
    void DisConnectWithClient(int connectFd);
    ErrorNo HandleAccept();
    ErrorNo HandleWriteEvent(int connectFd);
    ErrorNo HandleReadEvent(int connectFd);
    int socketFd_;
    int eventFd_;
    int serverPort_;
    Event event_;
    std::unordered_map<int, Connecter> connecter_;
    static constexpr int MAX_LISTEN_NUMBER = 10;
    static constexpr int MAX_EVENTS = 20;
    int timeEpollWaiting = 5000;  // 5000 : five seconds
};
}
#endif  // MINIRPC_SERVER_SERVER_H