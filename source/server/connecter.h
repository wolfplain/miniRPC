#ifndef MINIRPC_SERVER_CONNECTER_H
#define MINIRPC_SERVER_CONNECTER_H
#include "common/errorno.h"
#include "common/payload.h"
#include "rpc/rpc_service.h" // not good desgin

enum ConnectStatus {
    WAITING_SHAKE_MSG,
    WAITING_DATA_PKT,
    DATA_PKT_DONE,
};

class Connecter {
public:
    Connecter() : connectFd_(-1), status_(ConnectStatus::WAITING_SHAKE_MSG) {
    }
    Connecter(int connectFd) : connectFd_(connectFd), status_(ConnectStatus::WAITING_SHAKE_MSG) {
    }
    ~Connecter() = default;
    ErrorNo ProcessMessage(const std::string &content);
    ErrorNo SendResponse(const Response &rsp);
    void SetConnectStatus(const ConnectStatus &status) {
        status_ = status;
    }
    const ConnectStatus &GetConnectStatus() {
        return status_;
    }
    const Payload &GetPayload() {
        return payload_;
    }
    void ResetPayload();

private:
    ErrorNo ProcessShakeHanderMessage(const std::string &content);
    ErrorNo ProcessDataPktMessage(const std::string &content);
    ErrorNo Write(const std::string &responseMsg);

    int connectFd_;
    ConnectStatus status_;
    Payload payload_;
    std::unordered_map<std::string, std::string> headerInfo_;
};
#endif // MINIRPC_SERVER_CONNECTER_H