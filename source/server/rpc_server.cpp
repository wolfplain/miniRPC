#include "server/rpc_server.h"
#include "rpc/rpc_builder.h"
#include "common/log.h"

void RpcServer::OnStart(int connectFd) {
    LOG_D("connectfd:", connectFd);
}

void RpcServer::OnMessage(int connectFd, const std::string &message) {
    LOG_D("connectfd:", connectFd, " starts processing message");
    if (message.empty()) {
        return;
    }

    Request req;
    ErrorNo err = req.Parse(message);
    if (err != ErrorNo::SUCCESS) {
        LOG_E("service parse fail");
        return;
    }

    std::string svc;
    if (req.GetValue("svc", svc) != ErrorNo::SUCCESS) {
        LOG_E("svc not correct");
        return;
    }
    LOG_D("connectfd:", connectFd, "svc:", svc);

    RpcBuilder &builder = RpcBuilder::GetRpcBuilder();
    if (builder.CheckService(svc) != ErrorNo::SUCCESS) {
        LOG_E("service not exist");
        return;
    }

    Response rsp;
    RpcService *rpcService = builder[svc];
    if (rpcService == nullptr) {
        LOG_E("svc not exist");
        return;
    }
    err = rpcService->Process(req, rsp);
    if (err != ErrorNo::SUCCESS) {
        LOG_E("service process fail");
        return;
    }

    GetConnecter()[connectFd].SendResponse(rsp);
}

void RpcServer::OnClosed(int connectFd) {
    LOG_D("connectfd:", connectFd);
}

void RpcServer::OnError(int connectFd, ErrorNo errorNo) {
    LOG_D("connectfd:", connectFd, ",errorno:", (int)(errorNo));
}
