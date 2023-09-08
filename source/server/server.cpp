#include "server.h"
#include "common/log.h"

#include <cstring>
#include <memory>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <fcntl.h>


namespace Server {
ErrorNo WebSocketServer::Start()
{
    LOG_D("web socket server start...");
    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ == -1) {
        LOG_E("start server fail");
        return ErrorNo::FAILURE;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = serverPort_;
    serverAddr.sin_family = AF_INET;

    int ret = bind(socketFd_, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    if (ret == -1) {
        LOG_E("bind socket error");
        return ErrorNo::FAILURE;
    }

    ret = listen(socketFd_, MAX_LISTEN_NUMBER);
    if (ret == -1) {
        LOG_E("listen fail");
        return ErrorNo::FAILURE;
    }

    return ProcessConnect();
}

ErrorNo WebSocketServer::ProcessConnect()
{
    LOG_D("web socket server start process messages...");
    eventFd_ = event_.CreateEvent(socketFd_);
    if (eventFd_ < 0) {
        LOG_E("create event error");
        return ErrorNo::FAILURE;
    }

    if (event_.SetEvents(socketFd_, EventType::READ, EventOpt::ADD) != ErrorNo::SUCCESS) {
        LOG_E("start server event fail");
        return ErrorNo::DATA_INVALID;
    }

    for (;;) {
        int eventCnt = event_.GetEventNumber();
        LOG_D(eventCnt);
        for (int i = 0; i < eventCnt; i++) {
            int connectFd = event_.GetConnectFd(i);
            std::cout << connectFd << " socket:" << socketFd_ << " eventfd:" << eventFd_ << std::endl;
            ErrorNo errorNo = ErrorNo::SUCCESS;
            if (connectFd == socketFd_ && event_.IsReadEvent(i)) {
                HandleAccept();
                continue;  // keep process even if current connection is fails.
            }
            if (event_.IsReadEvent(i)) {
                errorNo = HandleReadEvent(connectFd);
            }
            if (event_.IsWriteEvent(i)) {
                errorNo = HandleWriteEvent(connectFd);
            }

            if (errorNo != ErrorNo::SUCCESS) {
                LOG_E("handle Message fail");
                DisConnectWithClient(connectFd);
            }
        }
    }
    return ErrorNo::SUCCESS;
}

ErrorNo WebSocketServer::HandleAccept()
{
    LOG_D("handle accept");
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int connectFd = accept(socketFd_, reinterpret_cast<struct sockaddr *>(&clientAddr), 
                           &clientLen);
    if (connectFd == -1) {
        LOG_E("accept fial");
        return ErrorNo::DATA_INVALID;
    }

    // if (-1 == fcntl(connectFd, O_NONBLOCK)) {
    //     LOG_E("accept fial");
    //     return ErrorNo::DATA_INVALID;
    // }

    // setnonblocking(connectFd);
    ErrorNo errorNo = event_.SetEvents(connectFd, EventType::READ, EventOpt::ADD);
    if (errorNo != ErrorNo::SUCCESS) {
        LOG_E("set events fail");
        return ErrorNo::FAILURE;
    }

    // errorNo = event_.SetEvents(connectFd, EventType::ET, EventOpt::MOD);
    // if (errorNo != ErrorNo::SUCCESS) {
    //     LOG_E("set events fail");
    //     return ErrorNo::FAILURE;
    // }
    LOG_E("connect fd:", connectFd);
    connecter_[connectFd] = Connecter(connectFd);
    return errorNo;
}


ErrorNo WebSocketServer::HandleReadEvent(int connectFd)
{
    LOG_D("process read operation ",connectFd);
    if (connecter_.count(connectFd) == 0) {
        LOG_E("invaild connecter:",connectFd);
        return ErrorNo::INVALID_PARAM;
    }
    uint64_t buffLen = 0;
    // Get the number of bytes that are immediately available for reading
    int ret = ioctl(connectFd, FIONREAD, &buffLen);
    if (ret == -1) {  //  || buffLen == 0
        LOG_E("get buffer length fail, len:",buffLen);
        return ErrorNo::SUCCESS;
    }

    std::cout << "bufflen :" << buffLen << std::endl;
    std::unique_ptr<char[]> content = std::make_unique<char[]>(buffLen);
    if (::read(connectFd, content.get(), buffLen) <= 0) {
        LOG_E("read data fail");
        return ErrorNo::FAILURE;
    }

    LOG_D(content.get());

    Connecter& connecter = connecter_.at(connectFd);
    ErrorNo errorNo = connecter.ProcessMessage(content.get());
    if (errorNo != ErrorNo::SUCCESS) {
        LOG_E("process msg fail");
        return errorNo;
    }

    OnMessage(connectFd, connecter.GetPayload().payload);

    if (connecter.GetConnectStatus() == ConnectStatus::DATA_PKT_DONE) {
        connecter.SetConnectStatus(ConnectStatus::WAITING_DATA_PKT);
        connecter.ResetPayload();
    }
    return ErrorNo::SUCCESS;
}

ErrorNo WebSocketServer::HandleWriteEvent(int connectFd)
{
    LOG_D("process write operation");
    if (connecter_.count(connectFd) == 0) {
        LOG_E("invaild connecter:", connectFd);
        return ErrorNo::INVALID_PARAM;
    }

    LOG_E("not ready yet");
    OnMessage(connectFd, "");
    return ErrorNo::SUCCESS;
}

void WebSocketServer::DisConnectWithClient(int connectFd)
{
    OnClosed(connectFd);
    (void)event_.SetEvents(connectFd, EventType::HUP, EventOpt::DEL);
    ::close(connectFd);
    connecter_.erase(connectFd);
}

std::unordered_map<int, Connecter> WebSocketServer::GetConnecter()
{
    return connecter_;
}

WebSocketServer::~WebSocketServer()
{
    (void)event_.SetEvents(socketFd_, EventType::HUP, EventOpt::DEL);
    ::close(socketFd_);
}
}